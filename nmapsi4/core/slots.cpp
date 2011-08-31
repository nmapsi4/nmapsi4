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

#include "../mainwin.h"

void nmapClass::setNmapsiSlot()
{

    connect(portCombo, SIGNAL(activated(const QString&)),
            this, SLOT(update_portCombo()));  // portCombo slot
    connect(comboScan, SIGNAL(activated(const QString&)),
            this, SLOT(update_scanCombo()));  // comboScan slot

    connect(checkInputFile, SIGNAL(toggled(bool)),
            this, SLOT(update_inputcheck()));  // input check
    connect(monitorStopAllScanButt, SIGNAL(clicked()),
            this, SLOT(stop_scan()));    // stop scan button slot

    // discover
    connect(checkTcpPing, SIGNAL(toggled(bool)),
            this, SLOT(update_discover()));  // discover check
    connect(checkTcpSyn, SIGNAL(toggled(bool)),
            this, SLOT(update_discover()));  // discover check
    connect(checkUdpPing, SIGNAL(toggled(bool)),
            this, SLOT(update_discover()));  // discover check

    // action slot
    connect(action_Quit, SIGNAL(triggered()),
            this, SLOT(exit()));    // file action menu
    connect(action_Quit_2, SIGNAL(triggered()),
            this, SLOT(exit()));    // file action menu
    connect(action_About, SIGNAL(triggered()),
            this, SLOT(about()));    // about action menu
    connect(actionAbout_Qt, SIGNAL(triggered()),
            this, SLOT(about_qt()));    // about action menu
    connect(action_Scan_menu, SIGNAL(triggered()),
            this, SLOT(startScan()));    // about action menu
    connect(actionClear_History, SIGNAL(triggered()),
            this, SLOT(listClear()));
    connect(buttonHClear, SIGNAL(clicked()),
            hostEdit, SLOT(clearEditText()));
    connect(action_Scan_2, SIGNAL(triggered()),
            this, SLOT(startScan()));    // about action menu
    connect(buttonBrowser, SIGNAL(clicked()),
            this, SLOT(input_browser()));    // Input file Browser slot

    // Timing Slot (Enable and Disable spinBox)
    connect(TcheckIpv4ttl, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckMinPar, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckMaxPar, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckInitRtt, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckMinRtt, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckMaxRtt, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckHostTime, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckScanDelay, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));

    //Options
    connect(checkBoxDevice, SIGNAL(toggled(bool)),
            this, SLOT(update_options()));
    connect(checkDecoy, SIGNAL(toggled(bool)),
            this, SLOT(update_options()));
    connect(checkSpoof, SIGNAL(toggled(bool)),
            this, SLOT(update_options()));
    connect(checkSourcePort, SIGNAL(toggled(bool)),
            this, SLOT(update_options()));
    connect(comboVerbosity, SIGNAL(activated(const QString&)),
            this, SLOT(update_comboVerbosity()));

    connect(actionProfile, SIGNAL(triggered()),
            this, SLOT(startProfile_ui()));
    connect(action_Save_As, SIGNAL(triggered()),
            this, SLOT(saveAsLog()));
    connect(actionSave, SIGNAL(triggered()),
            this, SLOT(saveLog()));
    connect(actionSave_As_Menu, SIGNAL(triggered()),
            this, SLOT(saveAsLog()));
    connect(actionSave_Menu, SIGNAL(triggered()),
            this, SLOT(saveLog()));
    connect(actionFullS, SIGNAL(triggered()),
            this, SLOT(checkFullScreen()));
    connect(actionMenuBar, SIGNAL(triggered()),
            this, SLOT(updateMenuBar()));
    connect(actionShow_Status_Bar, SIGNAL(triggered()),
            this, SLOT(showStatusBar()));
    connect(optionAccept, SIGNAL(clicked()),
            this, SLOT(resetPar()));
    connect(Bdetails, SIGNAL(clicked()),
            this, SLOT(detailsOptions()));
    connect(Bnss, SIGNAL(clicked()),
            this, SLOT(nssOptions()));
    connect(Bbook, SIGNAL(clicked()),
            this, SLOT(bookOptions()));
    connect(BBPar, SIGNAL(clicked()),
            this, SLOT(bookOptionsPar()));
    
    connect(globalButton, SIGNAL(clicked()),
            this, SLOT(saveGlobalProfile()));
    connect(removeGlobalButton, SIGNAL(clicked()),
            this, SLOT(removeGlobalProfile()));

    // Obj parser runtime
    connect(treeMain, SIGNAL( itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(runtimePars(QTreeWidgetItem*, int)));
    connect(treeTraceroot, SIGNAL( itemActivated(QTreeWidgetItem*, int)),
            this, SLOT(runtimeTraceroutePars(QTreeWidgetItem*, int)));
    connect(GItree, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuServiceMain()));

    // BookMark signals
    connect(treeLogH, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuScanBook()));
    connect(treeBookVuln, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuVulnBook()));
    connect(treeBookPar, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuParBook()));


    // Vuln signal
    connect(actSearch, SIGNAL(triggered()),
            this, SLOT(searchVulnNG()));
    connect(viewVuln, SIGNAL(loadProgress(int)),
            progressWeb, SLOT(setValue(int)));
    connect(viewVuln, SIGNAL(loadFinished(bool)),
            this, SLOT(vulnPostScan()));
    connect(actBack, SIGNAL(triggered()),
            this, SLOT(tabWebBack()));
    connect(actForward, SIGNAL(triggered()),
            this, SLOT(tabWebForward()));
    connect(actStop, SIGNAL(triggered()),
            this, SLOT(tabWebStop()));
    connect(comboVuln, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(updateComboVuln(const QString&)));
    connect(actionAdd_Bookmark, SIGNAL(triggered()),
            this, SLOT(saveBookMarks()));
    connect(action_Add_BookmarkToolBar, SIGNAL(triggered()),
            this, SLOT(saveBookMarks()));
    connect(addBookPar, SIGNAL(clicked()),
            this, SLOT(startAddParBook_ui()));
    connect(comboPar, SIGNAL(activated(const QString&)),
            this, SLOT(slotParSelected()));
    connect(comboHostBook, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(slotHostSelected()));

    connect(tWresult, SIGNAL(tabCloseRequested(int)),
            this,SLOT(closeVulnTab(int)));
    connect(comboVulnRis->lineEdit(), SIGNAL(cursorPositionChanged(int,int)),
            this, SLOT(updateFontHostVuln()));
    connect(comboVulnRis->lineEdit(), SIGNAL(returnPressed()),
            this, SLOT(searchVulnNG()));
    connect(addUrlButt, SIGNAL(clicked()),
            this, SLOT(startAddUrl_ui()));
    connect(removeUrlButt, SIGNAL(clicked()),
            this, SLOT(removeUrlToBookmarks()));

    // main session
    connect(scanSez, SIGNAL(triggered()),
            this, SLOT(updateSezScan()));
    connect(actionScan_section, SIGNAL(triggered()),
            this, SLOT(updateSezScan()));
    connect(logSez, SIGNAL(triggered()),
            this, SLOT(updateSezLog()));
    connect(actionLog_section, SIGNAL(triggered()),
            this, SLOT(updateSezLog()));
    connect(vulnSez, SIGNAL(triggered()),
            this, SLOT(updateSezVuln()));
    connect(discoverSez, SIGNAL(triggered()),
            this, SLOT(updateDiscoverVuln()));
    connect(actionVulnerabilities_section, SIGNAL(triggered()),
            this, SLOT(updateSezVuln()));
    connect(actionSection_Discover, SIGNAL(triggered()),
            this, SLOT(updateDiscoverVuln()));
    connect(nssAct, SIGNAL(triggered()),
            this, SLOT(NSSCheck()));
    connect(parAct, SIGNAL(triggered()),
            this, SLOT(parAdv()));
    connect(actTabLook, SIGNAL(triggered()),
            this, SLOT(updateTabLook()));
    connect(actTabTrace, SIGNAL(triggered()),
            this, SLOT(updateTabTrace()));
    connect(actTabMonitor, SIGNAL(triggered()),
            this, SLOT(updateTabMonitor()));
    connect(hostEdit->lineEdit(), SIGNAL(returnPressed()),
            this, SLOT(startScan()));
    connect(hostEdit->lineEdit(), SIGNAL(cursorPositionChanged(int,int)),
            this, SLOT(updateFontHost()));
    // monitor events
    connect(scanMonitor, SIGNAL( itemSelectionChanged()),
            this, SLOT(monitorRuntimeEvent()));
    connect(monitorStopCurrentScanButt, SIGNAL(clicked()),
            this, SLOT(monitorStopCurrentScan()));
    connect(monitorDetailsScanButt, SIGNAL(clicked()),
            this, SLOT(monitorShowDetails()));

    // nse category
    connect(nseComboScript, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateNseOptionScript(int)));
    connect(nseActiveBut, SIGNAL(clicked()),
            this, SLOT(nseTreeActiveItem()));
    connect(nseRemoveBut, SIGNAL(clicked()),
            this, SLOT(nseTreeRemoveItem()));
    connect(nseResetBut, SIGNAL(clicked()),
            this, SLOT(nseTreeResetItem()));
    connect(nseTreeAvail, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(requestNseHelp(QTreeWidgetItem*,int)));
    connect(nseTreeActive, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(requestNseHelp(QTreeWidgetItem*,int)));
    connect(nseFixedSButt, SIGNAL(clicked()),
            this, SLOT(resetPar()));
    connect(searchButtHelp, SIGNAL(clicked()),
            this, SLOT(requestNseScriptHelp()));
    
    // action help menu (browser call)
    connect(actionReport_Bug, SIGNAL(triggered()),
            this, SLOT(show_bugUrl()));
    connect(actionVisit_Website, SIGNAL(triggered()),
            this, SLOT(show_homepageUrl()));
    connect(actionDocumentation, SIGNAL(triggered()),
            this, SLOT(show_documentationUrl()));
    connect(actionDonate_Money, SIGNAL(triggered()),
            this, SLOT(show_donateUrl()));
    
    // Discover
    connect(comboDiscover, SIGNAL(activated(const QString&)),
            this, SLOT(discoverIp(const QString&)));
    connect(startDiscoverButt, SIGNAL(clicked()),
            this, SLOT(discoverIpState()));
    connect(stopDiscoverButt, SIGNAL(clicked()),
            this, SLOT(stopDiscover()));
    connect(treeDiscover, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(runtimeScanDiscover()));
    connect(discoverScanButt, SIGNAL(clicked()),
            this, SLOT(callScanDiscover()));
}
