/***************************************************************************
 *   Copyright (C) 2007-2012 by Francesco Cecconi                          *
 *   francesco.cecconi@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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
    treeMain->setColumnWidth(0, 200);
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_scanWidget(new ScanWidget(this)),
    m_mainTabWidget(new QTabWidget(this)),
    m_userId(0)
{
    QTimer::singleShot(0, this, SLOT(initObject()));
}

void MainWindow::initObject()
{
#if !defined(Q_WS_WIN)
    m_userId = getuid();
#endif

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
    loadDefaultProfile();

    // load first profile
    updateMenuBar();

    // load quick combo items
    loadScanProfile();
    updateComboBook();

    // connect slots
    connect(m_scanWidget->buttonHostClear, SIGNAL(clicked()),
            this, SLOT(clearHostnameCombo()));
    connect(m_scanWidget->buttonParametersClear, SIGNAL(clicked()),
            this, SLOT(clearParametersCombo()));
    connect(m_scanWidget->comboParametersProfiles, SIGNAL(activated(QString)),
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

    updateScanSection();

#if defined(Q_WS_MAC)
    // w/o show() call, mainwindow is not visible in mac osx
    show();
#endif
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

    pManager.data()->exec();

    if (!pManager.isNull()) {
        delete pManager.data();
    }
}

void MainWindow::editProfile()
{
    QWeakPointer<ProfilerManager> pManager = new ProfilerManager(m_scanWidget->comboParametersProfiles->currentText(), m_scanWidget->comboAdv->currentText(), this);

    connect(pManager.data(), SIGNAL(doneParBook(QString,QString)),
            m_bookmark, SLOT(saveParametersToBookmarks(QString,QString)));

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
            // TODO: wrong address
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

    QStringList parameters = getParameters();

    QHostAddress address(hostname);

    if ((address.protocol() == QAbstractSocket::IPv6Protocol) && !containsParameter("-6")) {
        // append "-6" parameter
        parameters << "-6";
        updateComboParametersFromList(parameters);
    } else if ((address.protocol() == QAbstractSocket::IPv4Protocol) && containsParameter("-6")) {
        // remove "-6" parameter
        parameters.removeAll("-6");
        updateComboParametersFromList(parameters);
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

MainWindow::~MainWindow()
{
}

#if defined(USE_KDELIBS)

void MainWindow::hideKWidget()
{
    m_kWidgetNotification->hide();
    m_kWidgetNotification->setText(QString(""));
}

#endif
