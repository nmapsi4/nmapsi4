/*
Copyright 2007-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

ScanWidget::ScanWidget(QWidget* parent): QWidget(parent)
{
    setupUi(this);

    // set default value in combo editable
    hostEdit->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    hostEdit->insertItem(0, tr("Host(s) to scan (ip/dns or incremental - ex. 192.168.1.1/20)"));
    // preload mainwindow info
    listWscan->setIconSize(QSize(22, 22));
    listScanError->setIconSize(QSize(22, 22));
    listScan->setIconSize(QSize(22, 22));
    treeTraceroot->setIconSize(QSize(22, 22));
    treeNSS->setIconSize(QSize(22, 22));
    treeMain->setIconSize(QSize(22, 22));
    treeLookup->setIconSize(QSize(22, 22));
    // set tree default settings
    treeTraceroot->setColumnWidth(1, 100);
    treeTraceroot->setColumnWidth(2, 200);
    treeTraceroot->setColumnWidth(3, 200);
    treeMain->setColumnWidth(0, 100);
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_mainTabWidget(new QTabWidget(this)),
    m_userId(0)
{
    QTimer::singleShot(0, this, SLOT(initObject()));
}

void MainWindow::initObject()
{
#if !defined(Q_OS_WIN32)
    m_userId = getuid();
#endif

    // allocate memory for scan widget
    m_scanWidget = new ScanWidget(this);
    //allocate centralWidget with layout QVBoxLayout(centralWidget)
    QWidget *centralwidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralwidget);

#if defined(USE_KDELIBS)
    m_kWidgetNotification = new KMessageWidget(this);
    m_kWidgetNotification->setCloseButtonVisible(false);
    QAction* action = new QAction(m_kWidgetNotification);
    action->setText(tr("Close"));
    action->setIcon(QIcon::fromTheme("application-exit", QIcon(":/images/images/window-close.png")));
    m_kWidgetNotification->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(hideKWidget()));
    m_kWidgetNotification->hide();

    centralLayout->addWidget(m_kWidgetNotification);
#endif

    centralLayout->addWidget(m_mainTabWidget);
    centralwidget->setLayout(centralLayout);
    setCentralWidget(centralwidget);

    // declare and restore items from bookmark
    m_bookmark = new BookmarkManager(this);
    m_bookmark->restoreAllHistoryValues();

    m_monitor = new Monitor(this);
    m_utilities = new Utilities(this);
    m_parser = new ParserManager(this);
    m_vulnerability = new Vulnerability(this);
    m_discoverManager = new DiscoverManager(this);
    m_collections = new ActionManager(this);
    m_collections->createScanSectionBar();
    m_collections->createDiscoverBar();
    m_collections->createSectionsBar();
    m_collections->createToolButtonBar();
    m_collections->createVulnerabilityBar();

    // Set default properties
    m_collections->m_collectionsScanSection.value("scan-action")->setEnabled(false);
    m_collections->m_collectionsScanSection.value("bookmarkAddHost-action")->setEnabled(false);
    m_collections->m_collectionsScanSection.value("bookmarkAddService-action")->setEnabled(false);
    m_collections->disableVulnerabilityToolBar();
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(true);
    menuBar()->setContextMenuPolicy(Qt::PreventContextMenu);
    m_collections->m_collectionsScanSection.value("bookmarkAddVulnUrl-action")->setVisible(false);
    syncSettings();

    // create mainwindow Qsplitter
    setDefaultSplitter();
    // restore saved settings
    restoreSettings();
    updateCompleter();
    m_profileHandler = new ProfileHandler(this, m_userId, m_savedProfileIndex);
    m_profileHandler->loadDefaultProfile();

    // load first profile
    updateMenuBar();

    // load quick combo items
    buildScanProfileList();
    updateComboBook();

    // connect slots
    connect(m_scanWidget->buttonHostClear, SIGNAL(clicked()),
            this, SLOT(clearHostnameCombo()));
    connect(m_scanWidget->pushButtonLoadFromFile, SIGNAL(clicked()),
            this, SLOT(loadTargetListFromFile()));
    connect(m_scanWidget->buttonParametersClear, SIGNAL(clicked()),
            m_profileHandler, SLOT(clearParametersCombo()));
    connect(m_scanWidget->comboParametersProfiles, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(comboParametersSelectedEvent()));
    connect(m_scanWidget->comboHostBook, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(quickAddressSelectionEvent()));
    connect(m_scanWidget->hostEdit->lineEdit(), SIGNAL(returnPressed()),
            this, SLOT(startScan()));
    connect(m_scanWidget->hostEdit->lineEdit(), SIGNAL(cursorPositionChanged(int,int)),
            this, SLOT(updateComboHostnameProperties()));
    // monitor events
    connect(m_monitor, SIGNAL(monitorUpdated(int)),
            this, SLOT(updateScanCounter(int)));

    // create welcome qml view
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 163, QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_welcomeQmlView = new QDeclarativeView(this);
    QVBoxLayout *qmlWelcomeLayout = new QVBoxLayout(m_welcomeQmlView);
    m_welcomeQmlView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    qmlWelcomeLayout->addItem(verticalSpacer);
    m_welcomeQmlView->rootContext()->setContextProperty("mainObject", this);
    m_welcomeQmlView->rootContext()->setContextProperty("version_number", VERSION);
    updateQmlScanHistory();

    m_welcomeQmlView->setSource(Package::qmlPath("mainWelcome"));

    // set saved profile or simply the default one
    m_scanWidget->comboParametersProfiles->setCurrentIndex(m_savedProfileIndex);
    updateWelcomeSection();

#if defined(Q_OS_MAC)
    // w/o show() call, mainwindow is not visible in mac osx
    show();
#endif
}

MainWindow::~MainWindow()
{
}

void MainWindow::startPreferencesDialog()
{
    QWeakPointer<PreferencesDialog> dialogPreference = new PreferencesDialog(this);

    connect(dialogPreference.data(), SIGNAL(accepted()),
            this, SLOT(syncSettings()));

    dialogPreference.data()->exec();

    if (!dialogPreference.isNull()) {
        delete dialogPreference.data();
    }
}

void MainWindow::newProfile()
{
    QWeakPointer<ProfilerManager> pManager = new ProfilerManager(this);

    connect(pManager.data(), SIGNAL(doneParBook(QString,QString)),
            m_bookmark, SLOT(saveParametersToBookmarks(QString,QString)));

    connect(pManager.data(), SIGNAL(doneQuickProfile(QStringList)),
            m_profileHandler, SLOT(updateComboParametersFromList(QStringList)));

    pManager.data()->exec();

    if (!pManager.isNull()) {
        delete pManager.data();
    }
}

void MainWindow::editProfile()
{
    QWeakPointer<ProfilerManager> pManager = new ProfilerManager(m_scanWidget->comboParametersProfiles->currentText(),
                                                                 m_scanWidget->comboAdv->currentText(), this);

    connect(pManager.data(), SIGNAL(doneParBook(QString,QString)),
            m_bookmark, SLOT(saveParametersToBookmarks(QString,QString)));

    connect(pManager.data(), SIGNAL(doneQuickProfile(QStringList)),
            m_profileHandler, SLOT(updateComboParametersFromList(QStringList)));

    pManager.data()->exec();

    if (!pManager.isNull()) {
        delete pManager.data();
    }
}

void MainWindow::linkCompleterToHostname()
{
    if (!m_collections->m_collectionsScanSection.value("bookmarkAddHost-action")->isEnabled()) {
        m_collections->m_collectionsScanSection.value("bookmarkAddHost-action")->setEnabled(true);
    }

    if (m_hostModel.isNull()) {
        return;
    }

    if (m_completer.isNull()) {
        m_completer = new QCompleter(m_hostModel.data(), this);
        m_completer.data()->setCompletionRole(QCompleter::InlineCompletion);
        m_completer.data()->setCaseSensitivity(Qt::CaseInsensitive);
        m_completer.data()->setWrapAround(false);
        m_scanWidget->hostEdit->setCompleter(m_completer.data());
    }
}

void MainWindow::takeHostFromBookmark()
{
    if (m_bookmark->m_scanBookmarkWidget->treeLogH->currentItem()) {
        updateComboHostnameProperties();
        m_scanWidget->hostEdit->insertItem(0, m_bookmark->m_scanBookmarkWidget->treeLogH->currentItem()->text(0));
        startScan();
    }
}

void MainWindow::quickAddressSelectionEvent()
{
    if (m_scanWidget->comboHostBook->currentIndex()) {
        updateComboHostnameProperties();
        m_scanWidget->hostEdit->insertItem(0, m_scanWidget->comboHostBook->currentText());
        // reset comboHostBook to default selection item (index=0)
        m_scanWidget->comboHostBook->setCurrentIndex(0);
    }
}

void MainWindow::startScan()
{
    if (m_scanWidget->hostEdit->currentText().isEmpty()) {
        QMessageBox::warning(this, "NmapSI4", tr("Insert an host/ip address to scan\n"), tr("Close"));
        return;
    }

    QString hostname = m_scanWidget->hostEdit->currentText();
    // check wrong address
    hostname = HostTools::clearHost(hostname);

    if (!m_monitor->monitorHostNumber()) {
        // clear details QHash
        m_monitor->clearHostMonitorDetails();
    }

    // check for ip range (x.x.x.x/x)
    if (hostname.contains("/") && !hostname.endsWith(QLatin1String("/")) && !hostname.contains("//")) {
        // is a ip list
        QStringList addressToken = hostname.split('/', QString::SkipEmptyParts);

        if (addressToken.isEmpty() || addressToken.size() < 2) {
            // TODO: wrong ip range
            return;
        }

        QHostAddress addressProtocol(addressToken[0]);

        if (addressProtocol.protocol() == QAbstractSocket::IPv6Protocol) {
            QMessageBox::warning(this, tr("Warning - Nmapsi4"),
                                 tr("IPv6 Protocol range scan is not yet supported."), tr("Close"));
            return;
        }

        QStringList ipfields = addressToken[0].split('.');
        int startIpRange = ipfields[3].toInt();
        int endIpRange = addressToken[1].toInt();

        for (int index = startIpRange; index <= endIpRange; index++) {
            ipfields[3].setNum(index);
            hostname = ipfields.join(".");

            if (!HostTools::isDns(hostname) || HostTools::isValidDns(hostname)) {
                addHostToMonitor(hostname);
            }
        }
        return;
    } else if (hostname.endsWith(QLatin1String("/"))) {
        hostname.remove('/');
    }

    //scan token DNS/IP parser
    if (hostname.contains(" ")) {
        // space delimiter
        QStringList addrPart_ = hostname.split(' ', QString::SkipEmptyParts);
        // check for only one space in hostname
        if (addrPart_.size() > 1) {
            // multiple ip or dns to scan
            for (int index = 0; index < addrPart_.size(); index++) {
                addrPart_[index] = HostTools::clearHost(addrPart_[index]);
                // check for lookup support
                if (!HostTools::isDns(addrPart_[index]) || HostTools::isValidDns(addrPart_[index])) {
                    addHostToMonitor(addrPart_[index]);
                }
            }
            return;
        }
        // remove all space on hostname
        hostname.remove(' ');
    }

    // single ip or dns
    if (!HostTools::isDns(hostname) || HostTools::isValidDns(hostname)) {
        addHostToMonitor(hostname);
    }

}

void MainWindow::addHostToMonitor(const QString hostname)
{
    // check for duplicate hostname in the monitor
    if (m_monitor->isHostOnMonitor(hostname)) {
        QMessageBox::warning(this, "NmapSI4", tr("Hostname already scanning\n"), tr("Close"));
        return;
    }

    m_bookmark->saveHostToBookmark(hostname, m_hostCache);
    updateCompleter();

    // default action
    m_monitor->m_monitorWidget->monitorStopAllScanButt->setEnabled(true);
    m_collections->disableSaveActions();

    QStringList parameters = m_profileHandler->getParameters();

    QHostAddress address(hostname);

    if ((address.protocol() == QAbstractSocket::IPv6Protocol) && !m_profileHandler->containsParameter("-6")) {
        // append "-6" parameter
        parameters << "-6";
        m_profileHandler->updateComboParametersFromList(parameters);
    } else if ((address.protocol() == QAbstractSocket::IPv4Protocol) && m_profileHandler->containsParameter("-6")) {
        // remove "-6" parameter
        parameters.removeAll("-6");
        m_profileHandler->updateComboParametersFromList(parameters);
    }

    // check for scan lookup
    switch (m_lookupType) {
    case Monitor::DisabledLookup:
        m_monitor->addMonitorHost(hostname, parameters, Monitor::DisabledLookup);
        break;
    case Monitor::InternalLookup:
        m_monitor->addMonitorHost(hostname, parameters, Monitor::InternalLookup);
        break;
    case Monitor::DigLookup:
        m_monitor->addMonitorHost(hostname, parameters, Monitor::DigLookup);
        break;
    }

    Notify::startButtonNotify(m_collections->m_collectionsButton.value("scan-sez"));
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if (m_monitor->monitorHostNumber()) {
        QMessageBox box;
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("<b>There are still active scan.</b>"));
        box.setInformativeText(tr("Do you want to close nmapsi4 anyway?"));
        box.setStandardButtons(QMessageBox::Cancel | QMessageBox::Close);
        box.setDefaultButton(QMessageBox::Cancel);
        int ret = box.exec();

        switch (ret) {
        case QMessageBox::Close:
            m_monitor->clearHostMonitor();
            saveSettings();
            event->accept();
            delete this;
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        }
    } else {
        m_monitor->clearHostMonitor();
        // Save Ui settings
        saveSettings();
        event->accept();
        delete this;
    }
}

void MainWindow::resizeVerticalSplitterEvent()
{
    if (!m_mainVerticalSplitter->sizes()[1]) {
        qDebug() << "Vertical QWidget details:: " << m_mainVerticalSplitter->sizes()[1];
        m_collections->m_collectionsButton.value("details-list")->setChecked(false);
    } else {
        m_collections->m_collectionsButton.value("details-list")->setChecked(true);
    }
}

void MainWindow::resizeHorizontalSplitterEvent()
{
    if (!m_mainHorizontalSplitter->sizes()[0]) {
        qDebug() << "Horizotal QWidget details:: " << m_mainHorizontalSplitter->sizes()[0];
        m_collections->m_collectionsButton.value("scan-list")->setChecked(false);
    } else {
        m_collections->m_collectionsButton.value("scan-list")->setChecked(true);
    }
}

void MainWindow::resizeHostDetailsWidgetEvent()
{
    if (!m_collections->m_collectionsButton.value("details-list")->isChecked()
            && m_mainVerticalSplitter->sizes()[1]) {
        m_detailsWidgetSize = m_mainVerticalSplitter->saveState();

        QList<int> size = m_mainVerticalSplitter->sizes();
        size[1] = 0;
        m_mainVerticalSplitter->setSizes(size);
    } else if (m_collections->m_collectionsButton.value("details-list")->isChecked()
               && !m_mainVerticalSplitter->sizes()[1]) {

        if (!m_detailsWidgetSize.isEmpty()) {
            // restore previous value
            m_mainVerticalSplitter->restoreState(m_detailsWidgetSize);
        } else {
            QList<int> size = m_mainVerticalSplitter->sizes();
            size[0] = 300; // ratio
            size[1] = 200;
            m_mainVerticalSplitter->setSizes(size);
        }
    }
}

void MainWindow::resizeScanListWidgetEvent()
{
    if (!m_collections->m_collectionsButton.value("scan-list")->isChecked()
            && m_mainHorizontalSplitter->sizes()[0]) {
        m_scanListWidgetSize = m_mainHorizontalSplitter->saveState();

        QList<int> size = m_mainHorizontalSplitter->sizes();
        size[0] = 0;
        m_mainHorizontalSplitter->setSizes(size);
    } else if (m_collections->m_collectionsButton.value("scan-list")->isChecked()
               && !m_mainHorizontalSplitter->sizes()[0]) {

        if (!m_scanListWidgetSize.isEmpty()) {
            // restore previous value
            m_mainHorizontalSplitter->restoreState(m_scanListWidgetSize);
        } else {
            QList<int> size = m_mainHorizontalSplitter->sizes();
            size[0] = 100; // ratio
            size[1] = 300;
            m_mainHorizontalSplitter->setSizes(size);
        }
    }
}

#if defined(USE_KDELIBS)

void MainWindow::hideKWidget()
{
    m_kWidgetNotification->hide();
    m_kWidgetNotification->setText(QString(""));
}

#endif

void MainWindow::syncSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");

    // load saved profile index
    m_savedProfileIndex = settings.value("savedProfileIndex", 0).toInt();
    m_hostCache = settings.value("hostCache", 10).toInt();

#if defined(Q_OS_WIN32)
    // disable lookup in MS windows
    m_lookupType = 0;
#else
    m_lookupType = settings.value("lookupType", 1).toInt();
#endif

    // restore actionMenuBar
    m_collections->m_collectionsScanSection.value("showmenubar-action")->setChecked(settings.value("showMenuBar", false).toBool());
    // update max parallel scan option
    m_monitor->updateMaxParallelScan();
}

void MainWindow::saveSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");

    settings.setValue("window/pos", pos());
    settings.setValue("window/geometry", saveGeometry());
    settings.setValue("hostCache", m_hostCache);
    settings.setValue("splitterSizes", m_mainHorizontalSplitter->saveState());
    settings.setValue("splitterSizesRight", m_mainVerticalSplitter->saveState());
    settings.setValue("showMenuBar", m_collections->m_collectionsScanSection.value("showmenubar-action")->isChecked());
    settings.setValue("savedProfileIndex", m_scanWidget->comboParametersProfiles->currentIndex());

    m_bookmark->syncSettings();
    m_discoverManager->syncSettings();
    m_parser->syncSettings();
    m_vulnerability->syncSettings();

// check and reset for settings file permission
#if !defined(Q_OS_WIN32)
    if (!m_userId) {
        QFileInfo fileInfo;

        fileInfo.setFile(settings.fileName());
        if ((!fileInfo.permissions().testFlag(QFile::WriteOther) && !fileInfo.ownerId())) {
            QFile::setPermissions(settings.fileName(), QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
                                  QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }

        QSettings settingsBookmark("nmapsi4", "nmapsi4_bookmark");
        fileInfo.setFile(settingsBookmark.fileName());
        if ((!fileInfo.permissions().testFlag(QFile::WriteOther) && !fileInfo.ownerId())) {
            QFile::setPermissions(settingsBookmark.fileName(), QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
                                  QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }
    }
#endif
}

void MainWindow::updateCompleter()
{
    if (!m_bookmark->isBookmarkHostListEmpty()) {
        if (!m_completer.isNull()) {
            QStringListModel *newModel = qobject_cast<QStringListModel*>(m_completer.data()->model());
            newModel->setStringList(m_bookmark->getHostListFromBookmark());
        } else if (m_hostModel.isNull()) {
            m_hostModel = new QStringListModel(m_bookmark->getHostListFromBookmark(), this);
        }
    }
}

void MainWindow::restoreSettings()
{
    // restore window position
    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    move(pos);

    if (settings.contains("window/geometry")) {
        restoreGeometry(settings.value("window/geometry").toByteArray());
    } else {
        resize(QSize(700, 500));
    }

    // restore state of the QAction's connected to splitter widget
    if (!settings.value("splitterSizes").toByteArray().isEmpty()) {
        m_mainHorizontalSplitter->restoreState(settings.value("splitterSizes").toByteArray());

        if (m_mainHorizontalSplitter->sizes()[0]) {
            m_collections->m_collectionsButton.value("scan-list")->setChecked(true);
        } else {
            m_collections->m_collectionsButton.value("scan-list")->setChecked(false);
        }
    } else {
        m_collections->m_collectionsButton.value("scan-list")->setChecked(true);
    }

    if (!settings.value("splitterSizesRight").toByteArray().isEmpty()) {
        m_mainVerticalSplitter->restoreState(settings.value("splitterSizesRight").toByteArray());

        if (m_mainVerticalSplitter->sizes()[1]) {
            m_collections->m_collectionsButton.value("details-list")->setChecked(true);
        } else {
            m_collections->m_collectionsButton.value("details-list")->setChecked(false);
        }
    } else {
        m_collections->m_collectionsButton.value("details-list")->setChecked(true);
    }
}

void MainWindow::setDefaultSplitter()
{
    // define default Ui splitter
    m_mainHorizontalSplitter = new QSplitter(this);
    m_mainVerticalSplitter = new QSplitter(this);

    connect(m_mainVerticalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(resizeVerticalSplitterEvent()));

    connect(m_mainHorizontalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(resizeHorizontalSplitterEvent()));

    m_mainHorizontalSplitter->setOrientation(Qt::Horizontal);
    m_mainHorizontalSplitter->addWidget(m_scanWidget->frameLeft);
    m_mainHorizontalSplitter->addWidget(m_scanWidget->frameCenter);
    m_mainVerticalSplitter->setOrientation(Qt::Vertical);
    m_mainVerticalSplitter->addWidget(m_scanWidget->tabWidget);
    m_mainVerticalSplitter->addWidget(m_scanWidget->frameRight);
    // insert splitter
    m_scanWidget->layout()->addWidget(m_mainHorizontalSplitter);
    m_scanWidget->frameCenter->layout()->addWidget(m_mainVerticalSplitter);
}

void MainWindow::setFullScreen()
{
    if (isFullScreen()) {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
        m_collections->m_collectionsScanSection.value("fullscreen-action")->setChecked(false);
    } else {
        setWindowState(windowState() | Qt::WindowFullScreen);
        m_collections->m_collectionsScanSection.value("fullscreen-action")->setChecked(true);
    }
}

void MainWindow::updateMenuBar()
{
    if (m_collections->m_collectionsScanSection.value("showmenubar-action")->isChecked()) {
        menuBar()->setVisible(true);
        m_collections->disableGlobalMenuToolBar();
    } else {
        menuBar()->setVisible(false);
        m_collections->enableGlobalMenuToolBar();
    }
}

void MainWindow::updateComboHostnameProperties()
{
    m_collections->m_collectionsScanSection.value("scan-action")->setEnabled(true);
    m_scanWidget->hostEdit->clear();
    m_scanWidget->hostEdit->setStyleSheet(QString::fromUtf8(""));
    bool signalState = m_scanWidget->hostEdit->lineEdit()->disconnect(SIGNAL(cursorPositionChanged(int,int)));

    if (!signalState) {
        return;
    }

    connect(m_scanWidget->hostEdit, SIGNAL(editTextChanged(QString)),
            this, SLOT(linkCompleterToHostname()));
}

void MainWindow::updateWelcomeSection()
{
    m_collections->m_collectionsButton.value("welcome-sez")->setChecked(true);
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(false);

    m_collections->disableScanSectionToolBar();
    m_collections->disableBookmarkToolBar();
    m_collections->disableVulnerabilityToolBar();
    //m_collections->disableGlobalMenuToolBar();
    m_collections->enableGlobalMenuToolBar();
    m_collections->m_discoverToolBar->setVisible(false);

    m_mainTabWidget->insertTab(0, m_welcomeQmlView,
                               QIcon(QString::fromUtf8(":/images/icons/128x128/nmapsi4.png")),
                               tr("Welcome"));
    m_mainTabWidget->setCurrentIndex(0);

    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_scanWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_scanBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_vulnerability->m_vulnerabilityWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_vulnBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_discoverManager->m_discoverWidget));

    m_collections->disableBottomUiToggleActions();
    m_collections->disableBookmarkMenu();

    updateQmlScanHistory();
}

void MainWindow::updateScanSection()
{
    m_collections->m_collectionsButton.value("welcome-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(true);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(false);
    m_collections->m_discoverToolBar->setVisible(false);
    m_collections->disableVulnerabilityToolBar();
    m_collections->enableBookmarkToolBar();
    m_collections->enableScanSectionToolBar();
    m_collections->enableGlobalMenuToolBar();

    m_mainTabWidget->insertTab(0, m_scanWidget, QIcon(QString::fromUtf8(":/images/images/network_local.png")),
                               "Scan");
    m_mainTabWidget->setCurrentIndex(0);

    if (!m_monitor->m_monitorWidget->isVisible()) {
        m_mainTabWidget->insertTab(1, m_monitor->m_monitorWidget, tr("Scan Monitor"));

        if (m_monitor->monitorHostNumber()) {
            m_mainTabWidget->setTabIcon(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget),
                                        QIcon::fromTheme("view-refresh", QIcon(":/images/images/reload.png")));
        } else {
            m_mainTabWidget->setTabIcon(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget),
                                        QIcon::fromTheme("utilities-system-monitor",
                                                         QIcon(":/images/images/utilities-log-viewer.png")));
        }

    }

    m_mainTabWidget->insertTab(m_mainTabWidget->count(), m_bookmark->m_scanBookmarkWidget,
                     QIcon::fromTheme("user-bookmarks", QIcon(":/images/images/bookmark_folder.png")),
                               tr("Bookmarks"));

    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_vulnerability->m_vulnerabilityWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_discoverManager->m_discoverWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_vulnBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_welcomeQmlView));

    // enable scan action
    m_collections->enableBottomUiToggleActions();
    m_collections->enableScanBookmarkMenu();
}

void MainWindow::updateVulnerabilitySection()
{
    Notify::clearButtonNotify(m_collections->m_collectionsButton.value("vuln-sez"));
    m_collections->m_collectionsButton.value("welcome-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(true);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(false);
    // main and action bar only in scan index
    m_collections->disableScanSectionToolBar();
    m_collections->enableVulnerabilityToolBar();
    m_collections->enableBookmarkToolBar();

    m_collections->enableGlobalMenuToolBar();
    m_collections->m_discoverToolBar->setVisible(false);

    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_scanBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_scanWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_discoverManager->m_discoverWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_welcomeQmlView));
    m_mainTabWidget->insertTab(0, m_vulnerability->m_vulnerabilityWidget,
                               QIcon(QString::fromUtf8(":/images/images/viewmag+.png")),
                               tr("Vulnerability"));

    m_mainTabWidget->insertTab(m_mainTabWidget->count(), m_bookmark->m_vulnBookmarkWidget,
                               QIcon::fromTheme("user-bookmarks", QIcon(":/images/images/bookmark_folder.png")),
                               tr("Bookmarks"));

    m_mainTabWidget->setCurrentIndex(0);

    // disable scan action
    m_collections->disableBottomUiToggleActions();
    m_collections->enableVulnerabilityBookmarkMenu();
}

void MainWindow::updateDiscoverSection()
{
    m_collections->m_collectionsButton.value("welcome-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(true);
    // main and action bar only in scan index
    m_collections->disableScanSectionToolBar();
    m_collections->disableBookmarkToolBar();
    m_collections->disableVulnerabilityToolBar();
    m_collections->m_discoverToolBar->setVisible(true);
    m_collections->enableGlobalMenuToolBar();

    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_scanBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_scanWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_vulnerability->m_vulnerabilityWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_vulnBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_welcomeQmlView));
    m_mainTabWidget->insertTab(0, m_discoverManager->m_discoverWidget,
                               QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")),
                               tr("Network discover"));
    m_mainTabWidget->setCurrentIndex(0);

    // disable scan action
    m_collections->disableBottomUiToggleActions();
    m_collections->disableBookmarkMenu();
}

void MainWindow::updateComboBook()
{
    m_scanWidget->comboHostBook->clear();
    m_scanWidget->comboHostBook->insertItem(0, "Select Saved Host");

    for (int index = 0; index < m_bookmark->m_scanBookmarkWidget->treeLogH->topLevelItemCount(); index++) {
        m_scanWidget->comboHostBook->insertItem(
            1, m_bookmark->m_scanBookmarkWidget->treeLogH->topLevelItem(index)->text(0));
    }
}

void MainWindow::clearAll()
{
    // Host list
    m_parser->clearParserItems();
    m_scanWidget->listScanError->clear();
    m_scanWidget->treeMain->clear();
    m_scanWidget->treeLookup->clear();
    m_scanWidget->treeTraceroot->clear();
    m_scanWidget->treeHostDet->clear();
    m_scanWidget->GItree->clear();
    m_scanWidget->listWscan->clear();
    m_scanWidget->treeNSS->clear();
    m_scanWidget->listScan->clear();
    m_collections->m_collectionsScanSection.value("clearHistory-action")->setEnabled(false);
    m_collections->disableSaveActions();

    m_scanWidget->comboScanLog->clear();
    m_scanWidget->comboScanLog->addItem(tr("Scan log parameters"));
}

void MainWindow::updateScanCounter(int hostNumber)
{
    if (hostNumber == 1) {
        m_mainTabWidget->setTabIcon(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget),
                                    QIcon::fromTheme("view-refresh", QIcon(":/images/images/reload.png")));
    }

    QString title(tr(" Active Scan ")
                  + QLatin1String("(")
                  + QString("%1").arg(m_monitor->monitorHostNumber())
                  + QLatin1String(")")
                  + " - Nmapsi4");

    setWindowTitle(title);
}

void MainWindow::clearHostnameCombo()
{
    // reset combo host bookmarks to default value
    m_scanWidget->comboHostBook->setCurrentIndex(0);
    m_scanWidget->hostEdit->clearEditText();
}

void MainWindow::buildScanProfileList()
{
    m_scanWidget->comboParametersProfiles->clear();

    QListIterator< QPair<QString, QString> > i(m_profileHandler->defaultScanProfile());
    while (i.hasNext()) {
        m_scanWidget->comboParametersProfiles->insertItem(
            m_scanWidget->comboParametersProfiles->count() + 1, i.next().first);
    }

    m_scanWidget->comboParametersProfiles->insertSeparator(m_scanWidget->comboParametersProfiles->count() + 1);

    // value from treeWidget parameters
    for (int index = 0; index < m_bookmark->m_scanBookmarkWidget->treeBookPar->topLevelItemCount(); index++) {
        m_scanWidget->comboParametersProfiles->addItem(
            m_bookmark->m_scanBookmarkWidget->treeBookPar->topLevelItem(index)->text(1));
    }

    comboParametersSelectedEvent();
}

void MainWindow::comboParametersSelectedEvent()
{
    // insert profile from comboPar to comboAdv
    int currentProfileIndex = m_scanWidget->comboParametersProfiles->currentIndex();

    // if not 0
    QList< QPair<QString, QString> > listProfileModel = m_profileHandler->defaultScanProfile();
    m_scanWidget->comboAdv->clear();

    if (currentProfileIndex <= listProfileModel.size()) {
        QListIterator< QPair<QString, QString> > i(m_profileHandler->defaultScanProfile());
        while (i.hasNext()) {
            QPair<QString, QString> profile = i.next();
            if (profile.first.contains(m_scanWidget->comboParametersProfiles->currentText())) {
                // call static default profile for check
                m_scanWidget->comboAdv->insertItem(0, profile.second);
                break;
            }
        }
    } else {
        // saved user profile
        QList<QTreeWidgetItem *> resultList_ = m_bookmark->m_scanBookmarkWidget->treeBookPar->findItems(
            m_scanWidget->comboParametersProfiles->currentText(), Qt::MatchExactly, 1);
        m_scanWidget->comboAdv->insertItem(0, resultList_[0]->text(0));
    }
}

void MainWindow::resetComboParameters()
{
    m_scanWidget->comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboParametersSelectedEvent();
}


void MainWindow::updateQmlScanHistory()
{
    QString hostScanned;

    for (int index = 0; index < 8 && index < m_hostModel.data()->stringList().size(); ++index) {
        hostScanned.append("+ " + m_hostModel.data()->stringList()[index] + "<hr/>");
    }

    if (hostScanned.isEmpty()) {
        hostScanned.append(tr("Empty history"));
    }

    m_welcomeQmlView->rootContext()->setContextProperty("historyListText", hostScanned);
}

void MainWindow::loadTargetListFromFile()
{
    const QString& fileName = QFileDialog::getOpenFileName(this, tr("Select the file"), "/home", "");

    QFile *targetFile = new QFile(fileName);

    if (!targetFile->open(QIODevice::ReadOnly))
    {
        return;
    }

    QTextStream buffer(targetFile);
    QString targetListOneLine;
    QString tmpLine;
    int lineCounter=0;

    while (!buffer.atEnd())
    {
        tmpLine = buffer.readLine();
        lineCounter++;

        if ((lineCounter >= 2) && (!tmpLine.startsWith(QLatin1String(" ")))) {
            targetListOneLine.append(' ');
        }

        targetListOneLine.append(tmpLine);
    }

    // fix wrong list format (target1,target2,target3)
    if (targetListOneLine.contains(',')) {
        targetListOneLine.replace(',',' ');
    }

    qDebug() << "Load list from file:: " << targetListOneLine;

    delete targetFile;

    // to load the host list in the hostname line edit
    updateComboHostnameProperties();
    m_scanWidget->hostEdit->insertItem(0, targetListOneLine);
}
