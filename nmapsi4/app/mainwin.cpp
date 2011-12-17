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

nmapClass::nmapClass() : uid(0), _logFilePath(0)
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

    _monitor = new monitor(scanMonitor);
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
    optionListCreate();
    // check dig support
    dig *digC  = new dig();
    digC->checkDigSupport(digSupported);
    delete digC;
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
    // restore value with uid check
    rootMode();

#ifdef Q_WS_WIN
    // lookup fails on MS Windows
    _collectionsButton.value("tab-look-act")->setChecked(false);
    _collectionsButton.value("tab-look-act")->setEnabled(false);
    LookupEnabled = false;
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
    updateComboPar();
    updateComboBook();
    _vulnerability->updateComboWebV();

    // call discover startup, NPING is REQUIRED
    _discoverManager = new discoverManager(this);
    _discoverManager->startDiscover();
    _discoverManager->defaultDiscoverProbes();

    // connect slots
    setNmapsiSlot();
}

nmapClass::~nmapClass()
{
    freelist<dig*>::itemDeleteAll(digLookupList);
    freelist<lookUpT*>::itemDeleteAll(internealLookupList);
    freelist<QTreeWidgetItem*>::itemDeleteAll(mainTreeElem);
    freemap<QString,QPushButtonOrientated*>::itemDeleteAll(_collectionsButton);
    delete _discoverManager;
    delete _monitor;
    delete _utilities;
    delete _parser;
    delete progressScan;
    delete _logFilePath;
    delete _completer;
    delete _completerVuln;
    delete _vulnerability;
    delete _nseManager;
    delete _hostModel;
    delete _vulnModel;
    delete bW;
    delete cW;
    delete menuSetup->menu();
    delete menuSetup;
}

