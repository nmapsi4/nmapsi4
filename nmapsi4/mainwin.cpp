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

nmapClass::nmapClass()
    : uid(0),
      PFile(NULL),
      scanCounter(0),
      labelVersion(NULL),
      userMode(NULL)
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

#ifdef Q_WS_WIN
    // lookup fails on MS Windows
    tabWidget->removeTab(3);
#endif
    tWresult->setTabsClosable(true);
    tWresult->removeTab(0);

    createBar();
    createToolButtonSetup();
    setNmapsiSlot();
    // Set default properties
    setDefaultAction();
    // preload mainwindow info
    setTreeWidgetValues();
    checkProfile();
    optionListCreate();
    // check dig support
    digSupport *digC  = new digSupport();
    digC->checkDigSupport(digSupported);
    delete digC;
    // check nmap version
    checkNmapVersion();
    // set tree default settings
    setTreeSettings();
    // create mainwindow Qsplitter
    setDefaultSplitter();
    // restore saved settings
    restoreSettings();
    // load history items
    loadHistoryDefault();
    // restore value with uid check
    rootMode();
    nssAct->setChecked(NSSsupport); // set NSS support
    NSSCheck();
    parAct->setChecked(ADVSupport); // set ADV support
    parAdv();
    actTabMonitor->setChecked(MonitorEnabled);
    updateTabMonitor();
    actTabLook->setChecked(LookupEnabled);
    updateTabLook();
    actTabTrace->setChecked(TraceEnabled);
    updateTabTrace();
    updateMenuBar();
    // load quick combo items
    updateComboPar();
    updateComboBook();
    updateComboWebV();
    // call discover startup, NPING is REQUIRED
    startDiscover();
    // set modes probe in discover
    defaultDiscoverProbes();
}

nmapClass::~nmapClass()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(itemListScan);
    freelist<digSupport*>::itemDeleteAll(digLookupList);
    freelist<lookUpT*>::itemDeleteAll(internealLookupList);
    freelist<QTreeWidgetItem*>::itemDeleteAll(monitorElem);
    freelist<QTreeWidgetItem*>::itemDeleteAll(mainTreeElem);
    freelist<QTreeWidgetItem*>::itemDeleteAll(itemNseActive);
    freelist<QTreeWidgetItem*>::itemDeleteAll(itemNseAvail);
    freelist<parserObj*>::itemDeleteAll(parserObjList);
    freelist<parserObjUtil*>::itemDeleteAll(parserObjUtilList);
    freelist<QWebView*>::itemDeleteAll(webViewList);
    scanHashListFlow.clear();
    cleanDiscovery();
    delete progressScan;
    delete PFile;
    delete labelVersion;
    delete userMode;
    delete scanSez;
    delete logSez;
    delete vulnSez;
    delete nssAct;
    delete parAct;
    delete actBack;
    delete actForward;
    delete actStop;
    delete bW;
    delete cW;
    delete menuSetup->menu();
    delete menuSetup;
}

