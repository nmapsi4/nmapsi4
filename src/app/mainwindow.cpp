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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_userId(0)
{
    initGUI();
    QTimer::singleShot(0, this, SLOT(initObject()));
}

void MainWindow::initGUI()
{
    setupUi(this);
    // set default value in combo editable
    defaultComboValues();
}

void MainWindow::initObject()
{
#if !defined(Q_WS_WIN)
    m_userId = getuid();
#endif

    // preload mainwindow info
    setTreeWidgetValues();

    // declare and restore items from bookmark
    m_bookmark = new BookmarkManager(this);
    m_bookmark->restoreAllHistoryValues();

    m_monitor = new Monitor(this);
    m_utilities = new Utilities(this);
    m_parser = new ParserManager(this);
    m_vulnerability = new Vulnerability(this);
    m_discoverManager = new DiscoverManager(this);
    m_collections = new ActionManager(this);

    m_collections->createSectionsBar();
    m_collections->createToolButtonBar();

    // Set default properties
    setDefaultAction();
    syncSettings();

    // set tree default settings
    setTreeSettings();
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

    // call discover startup, NPING is REQUIRED
    m_discoverManager->startDiscover();
    m_discoverManager->defaultDiscoverProbes();
    m_collections->createDiscoverBar();

    // connect slots
    connect(action_Quit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(action_Scan_menu, SIGNAL(triggered()),
            this, SLOT(startScan()));
    connect(actionClear_History, SIGNAL(triggered()),
            this, SLOT(clearAll()));
    connect(buttonHostClear, SIGNAL(clicked()),
            this, SLOT(clearHostnameCombo()));
    connect(buttonParametersClear, SIGNAL(clicked()),
            this, SLOT(clearParametersCombo()));
    connect(action_Scan_2, SIGNAL(triggered()),
            this, SLOT(startScan()));
    connect(actionProfile, SIGNAL(triggered()),
            this, SLOT(startPreferencesDialog()));
    connect(actionNew_Profile, SIGNAL(triggered()),
            this, SLOT(newProfile()));
    connect(actionEdit_Profile, SIGNAL(triggered()),
            this, SLOT(editProfile()));
    connect(actionFullScreen, SIGNAL(triggered()),
            this, SLOT(setFullScreen()));
    connect(actionMenuBar, SIGNAL(triggered()),
            this, SLOT(updateMenuBar()));
    connect(comboParametersProfiles, SIGNAL(activated(QString)),
            this, SLOT(comboParametersSelectedEvent()));
    connect(comboHostBook, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(quickAddressSelectionEvent()));
    connect(actionScan_section, SIGNAL(triggered()),
            this, SLOT(updateSezScan()));
    connect(actionVulnerabilities_section, SIGNAL(triggered()),
            this, SLOT(updateSezVuln()));
    connect(actionSection_Discover, SIGNAL(triggered()),
            this, SLOT(updateSezDiscover()));
    connect(hostEdit->lineEdit(), SIGNAL(returnPressed()),
            this, SLOT(startScan()));
    connect(hostEdit->lineEdit(), SIGNAL(cursorPositionChanged(int,int)),
            this, SLOT(updateComboHostnameProperties()));
    // monitor events
    connect(m_monitor, SIGNAL(monitorUpdated(int)),
            this, SLOT(updateScanCounter(int)));

    updateSezScan();
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
    QWeakPointer<ProfilerManager> pManager = new ProfilerManager(comboParametersProfiles->currentText(), comboAdv->currentText(), this);

    connect(pManager.data(), SIGNAL(doneParBook(QString,QString)),
            m_bookmark, SLOT(saveParametersToBookmarks(QString,QString)));

    pManager.data()->exec();

    if (!pManager.isNull()) {
        delete pManager.data();
    }
}

void MainWindow::linkCompleterToHostname()
{
    if (!actionAdd_Bookmark->isEnabled()) {
        actionAdd_Bookmark->setEnabled(true);
    }

    if (m_hostModel.isNull()) {
        return;
    }

    if (m_completer.isNull()) {
        m_completer = new QCompleter(m_hostModel.data(), this);
        m_completer.data()->setCompletionRole(QCompleter::InlineCompletion);
        m_completer.data()->setCaseSensitivity(Qt::CaseInsensitive);
        m_completer.data()->setWrapAround(false);
        hostEdit->setCompleter(m_completer.data());
    }
}

void MainWindow::takeHostFromBookmark()
{
    if (m_bookmark->m_scanBookmarkWidget->treeLogH->currentItem()) {
        updateComboHostnameProperties();
        hostEdit->insertItem(0, m_bookmark->m_scanBookmarkWidget->treeLogH->currentItem()->text(0));
        startScan();
    }
}

void MainWindow::quickAddressSelectionEvent()
{
    if (comboHostBook->currentIndex()) {
        updateComboHostnameProperties();
        hostEdit->insertItem(0, comboHostBook->currentText());
        // reset comboHostBook to default selection item (index=0)
        comboHostBook->setCurrentIndex(0);
    }
}

void MainWindow::startScan()
{
    if (hostEdit->currentText().isEmpty()) {
        QMessageBox::warning(this, "NmapSI4", tr("No Host Target\n"), tr("Close"));
        return;
    }

    QString hostname = hostEdit->currentText();
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
    action_Save_As->setEnabled(false);
    actionSave_As_Menu->setEnabled(false);
    actionSave->setEnabled(false);
    actionSave_Menu->setEnabled(false);

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
        Notify::clearButtonNotify(m_collections->m_collectionsButton.value("scan-list"));
    } else if (m_collections->m_collectionsButton.value("scan-list")->isChecked()
               && !m_mainHorizontalSplitter->sizes()[0]) {

        if (!m_scanListWidgetSize.isEmpty()) {
            // restore previous value
            m_mainHorizontalSplitter->restoreState(m_scanListWidgetSize);
            Notify::setCheckedNotify(m_collections->m_collectionsButton.value("scan-list"));
        } else {
            QList<int> size = m_mainHorizontalSplitter->sizes();
            size[0] = 100; // ratio
            size[1] = 300;
            m_mainHorizontalSplitter->setSizes(size);
            Notify::setCheckedNotify(m_collections->m_collectionsButton.value("scan-list"));
        }
    }
}

MainWindow::~MainWindow()
{
}
