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

MainWindow::MainWindow()
: uid(0)
{
    initGUI();
    QTimer::singleShot( 0, this, SLOT(initObject()) );
}

void MainWindow::initGUI()
{
    setupUi(this);
    // set default value in combo editable
    defaultComboValues();
}

void MainWindow::initObject()
{
#ifndef Q_WS_WIN
    uid = getuid();
#endif

    tWresult->setTabsClosable(true);
    tWresult->removeTab(0);

    // preload mainwindow info
    setTreeWidgetValues();

    _monitor = new monitor(scanMonitor,this);
    _utilities = new utilities(this);
    _parser = new parserManager(this);
    _vulnerability = new vulnerability(this);

    createBar();
    createToolButtonSetup();

    // Set default properties
    setDefaultAction();
    syncSettings();

    // set tree default settings
    setTreeSettings();
    // create mainwindow Qsplitter
    setDefaultSplitter();
    // restore saved settings
    restoreSettings();
    // load history items
    loadHistoryDefault();
    updateCompleter();

    loadDefaultProfile();

#ifdef Q_WS_WIN
    // lookup fails on MS Windows
    _collectionsButton.value("tab-look-act")->setChecked(false);
    _collectionsButton.value("tab-look-act")->setEnabled(false);
#else
    // removed dig runtime check, linux, mac and BSD.
    _collectionsButton.value("tab-look-act")->setChecked(lookupEnabled);
    updateTabLook();
#endif

    // load first profile
    _collectionsButton.value("tab-trace-act")->setChecked(TraceEnabled);
    updateTabTrace();
    updateMenuBar();

    // load quick combo items
    loadScanProfile();
    updateComboBook();
    _vulnerability->updateComboWebV();

    // call discover startup, NPING is REQUIRED
    _discoverManager = new discoverManager(this);
    _discoverManager->startDiscover();
    _discoverManager->defaultDiscoverProbes();

    // connect slots
    setNmapsiSlot();
}

void MainWindow::startProfile_ui()   // start preference UI
{
    QWeakPointer<preferencesDialog> dialogPreference = new preferencesDialog(this);

    connect(dialogPreference.data(), SIGNAL(accepted()),
            this, SLOT(syncSettings()));

    dialogPreference.data()->exec();

    if (!dialogPreference.isNull())
    {
        delete dialogPreference.data();
    }
}

void MainWindow::startProfilerManager()
{
    QWeakPointer<profilerManager> pManager = new profilerManager(this);

    connect(pManager.data(), SIGNAL(doneParBook(QString,QString)),
            this, SLOT(saveBookMarksPar(QString,QString)));

    pManager.data()->exec();

    if (!pManager.isNull())
    {
        delete pManager.data();
    }
}

void MainWindow::editProfile()
{
    QWeakPointer<profilerManager> pManager = new profilerManager(comboPar->currentText(),comboAdv->currentText(),this);

    connect(pManager.data(), SIGNAL(doneParBook(QString,QString)),
            this, SLOT(saveBookMarksPar(QString,QString)));

    pManager.data()->exec();

    if (!pManager.isNull())
    {
        delete pManager.data();
    }
}

void MainWindow::startScan()
{
    if (hostEdit->currentText().isEmpty())
    {
        QMessageBox::warning(this, "NmapSI4", tr("No Host Target\n"), tr("Close"));
        return;
    }

    QString hostname = hostEdit->currentText();
    // check wrong address
    hostname = hostTools::clearHost(hostname);

    // check for duplicate hostname in the monitor
    if (_monitor->isHostOnMonitor(hostname))
    {
        QMessageBox::warning(this, "NmapSI4", tr("Hostname already scanning\n"), tr("Close"));
        return;
    }

    if(!_monitor->monitorHostNumber())
    {
        // clear details QHash
       _monitor->clearHostMonitorDetails();
    }


    // check for ip list
    if(hostname.contains("/") && !hostname.contains("//"))
    {
        // is a ip list
        QStringList addrPart_ = hostname.split('/');
        QStringList ipBase_ = addrPart_[0].split('.');
        int ipLeft_ = ipBase_[3].toInt();
        int ipRight_ = addrPart_[1].toInt();
        // TODO limit parallel ip scan
        for(int index = ipLeft_; index <= ipRight_; index++)
        {
            ipBase_[3].setNum(index);
            hostname = ipBase_.join(".");

            if (!hostTools::isDns(hostname) || hostTools::isValidDns(hostname))
            {
                preScanLookup(hostname);
            }
        }
        return;
    }

    //scan token DNS/IP parser
    if(hostname.contains(" "))
    { // space delimiter
        QStringList addrPart_ = hostname.split(' ');
        addrPart_.removeAll("");
        // check for only one space in hostname
        if(addrPart_.size() > 1)
        {
            // multiple ip or dns to scan
            for(int index=0; index < addrPart_.size(); index++)
            {
                addrPart_[index] = hostTools::clearHost(addrPart_[index]);
                // check for lookup support
                if (!hostTools::isDns(addrPart_[index]) || hostTools::isValidDns(addrPart_[index]))
                {
                    preScanLookup(addrPart_[index]);
                }
            }
            return;
        }
        // remove all space on hostname
        hostname.remove(' ');
    }

    // single ip or dns after the move
    if (!hostTools::isDns(hostname) || hostTools::isValidDns(hostname))
    {
        preScanLookup(hostname);
    }

}

void MainWindow::preScanLookup(const QString hostname)
{
    // save ip or dns to history
    history *newHistory = new history("nmapsi4/cacheHost", hostCache);
    newHistory->addItemHistory(hostname);
    delete newHistory;

    updateCompleter();

    // default action
    monitorStopAllScanButt->setEnabled(true);
    action_Save_As->setEnabled(false);
    actionSave_As_Menu->setEnabled(false);
    actionSave->setEnabled(false);
    actionSave_Menu->setEnabled(false);

    QStringList parameters = loadExtensions();

    // check for scan lookup
    if (lookupEnabled)
    {
        switch (lookupType)
        {
        case monitor::DisabledLookup:
            _monitor->addMonitorHost(hostname, parameters, monitor::DisabledLookup);
            break;
        case monitor::InternalLookup:
            _monitor->addMonitorHost(hostname, parameters, monitor::InternalLookup);
            break;
        case monitor::DigLookup:
            _monitor->addMonitorHost(hostname, parameters, monitor::DigLookup);
            break;
        }
    }
}

void MainWindow::exit()
{
    _monitor->clearHostMonitor();
    // Save Ui settings
    saveSettings();
    close();
}

MainWindow::~MainWindow()
{
    freemap<QString,QPushButtonOrientated*>::itemDeleteAll(_collectionsButton);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeloghlist);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookparlist);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookvulnlist);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treewidgetvulnlist);
    delete _discoverManager;
    delete _monitor;
    delete _utilities;
    delete _parser;
    delete _vulnerability;
    delete _completer.data();
    delete _completerVuln.data();
    delete _hostModel.data();
    delete _vulnModel.data();
    delete bW;
    delete cW;
    delete progressScan;
    delete menuSetup->menu();
    delete menuSetup;
    delete m_profilerTool->menu();
    delete m_profilerTool;
    delete m_saveTool->menu();
    delete m_saveTool;
    delete m_bookmarksTool->menu();
    delete m_bookmarksTool;
}

