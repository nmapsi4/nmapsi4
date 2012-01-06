/***************************************************************************
 *   Copyright (C) 2007-2011 by Francesco Cecconi                          *
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

#include "mainwin.h"

nmapClass::nmapClass() : uid(0)
{
    initGUI();
    QTimer::singleShot( 0, this, SLOT(initObject()) );
}

void nmapClass::initGUI()
{
    setupUi(this);
    // set default value in combo editable
    defaultComboValues();
}

void nmapClass::initObject()
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

    // create and load nse values from file settings
    _nseManager = new nseManager(this);
    _nseManager->loadNseCategoryScript();

    createBar();
    createToolButtonSetup();

    // Set default properties
    setDefaultAction();
    checkProfile();

    // FIXME:: active hostEdit by default
    hostEdit->setEnabled(true);
    // set tree default settings
    setTreeSettings();
    // create mainwindow Qsplitter
    setDefaultSplitter();
    // restore saved settings
    restoreSettings();
    // load history items
    loadHistoryDefault();
    updateCompleter();

#ifdef Q_WS_WIN
    // lookup fails on MS Windows
    _collectionsButton.value("tab-look-act")->setChecked(false);
    _collectionsButton.value("tab-look-act")->setEnabled(false);
    LookupEnabled = false;
#else
    // removed dig runtime check, linux, mac and BSD.
    LookupEnabled = true;
#endif

    _collectionsButton.value("nss-act")->setChecked(NSSsupport);
    NSSCheck();
    // load first profile
    loadDefaultProfile();
    _collectionsButton.value("tab-look-act")->setChecked(LookupEnabled);
    updateTabLook();
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

void nmapClass::startProfile_ui()   // start preference UI
{
    preferencesDialog dialogPreference_(this);
    connect(&dialogPreference_, SIGNAL(accepted()),
            this, SLOT(checkProfile()));

    dialogPreference_.exec();
}

void nmapClass::startProfilerManager()
{
    profilerManager pManger(this);
    pManger.exec();
}

void nmapClass::exit()
{
    _monitor->clearHostMonitor();
    // Save Ui settings
    saveUiSettings();
    close();
}

nmapClass::~nmapClass()
{
    freemap<QString,QPushButtonOrientated*>::itemDeleteAll(_collectionsButton);
    delete _discoverManager;
    delete _monitor;
    delete _utilities;
    delete _parser;
    delete progressScan;
    delete _completer.data();
    delete _completerVuln.data();
    delete _vulnerability;
    delete _nseManager;
    delete _hostModel.data();
    delete _vulnModel.data();
    delete bW;
    delete cW;
    delete menuSetup->menu();
    delete menuSetup;
}

