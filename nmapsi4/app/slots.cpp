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

void nmapClass::setNmapsiSlot()
{

    connect(portCombo, SIGNAL(activated(QString)),
            this, SLOT(update_portCombo()));  // portCombo slot
    connect(comboScan, SIGNAL(activated(QString)),
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
            _utilities, SLOT(about()));    // about action menu
    connect(actionAbout_Qt, SIGNAL(triggered()),
            _utilities, SLOT(aboutQt()));    // about action menu
    connect(action_Scan_menu, SIGNAL(triggered()),
            this, SLOT(startScan()));    // about action menu
    connect(actionClear_History, SIGNAL(triggered()),
            this, SLOT(listClear()));
    connect(buttonHClear, SIGNAL(clicked()),
            hostEdit, SLOT(clearEditText()));
    connect(action_Scan_2, SIGNAL(triggered()),
            this, SLOT(startScan()));    // about action menu
    connect(buttonBrowser, SIGNAL(clicked()),
            this, SLOT(openBrowser()));    // Input file Browser slot

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
    connect(comboVerbosity, SIGNAL(activated(QString)),
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
    connect(treeMain, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            _parser, SLOT(showParserResult(QTreeWidgetItem*,int)));
    connect(treeTraceroot, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            _parser, SLOT(showParserTracerouteResult(QTreeWidgetItem*,int)));
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

    connect(actionAdd_Bookmark, SIGNAL(triggered()),
            this, SLOT(saveBookMarks()));
    connect(action_Add_BookmarkToolBar, SIGNAL(triggered()),
            this, SLOT(saveBookMarks()));
    connect(addBookPar, SIGNAL(clicked()),
            this, SLOT(startAddParBook_ui()));
    connect(comboPar, SIGNAL(activated(QString)),
            this, SLOT(slotParSelected()));
    connect(comboHostBook, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(slotHostSelected()));

    // main session
    connect(actionScan_section, SIGNAL(triggered()),
            this, SLOT(updateSezScan()));
    connect(actionLog_section, SIGNAL(triggered()),
            this, SLOT(updateSezLog()));
    connect(actionVulnerabilities_section, SIGNAL(triggered()),
            this, SLOT(updateSezVuln()));
    connect(actionSection_Discover, SIGNAL(triggered()),
            this, SLOT(updateDiscoverVuln()));
    connect(hostEdit->lineEdit(), SIGNAL(returnPressed()),
            this, SLOT(startScan()));
    connect(hostEdit->lineEdit(), SIGNAL(cursorPositionChanged(int,int)),
            this, SLOT(updateFontHost()));

    // monitor events
    connect(scanMonitor, SIGNAL(itemSelectionChanged()),
            this, SLOT(monitorRuntimeEvent()));
    connect(monitorStopCurrentScanButt, SIGNAL(clicked()),
            _monitor, SLOT(stopSelectedScan()));
    connect(monitorDetailsScanButt, SIGNAL(clicked()),
            _monitor, SLOT(showSelectedScanDetails()));
    connect(_monitor, SIGNAL(monitorUpdated(int)),
            this, SLOT(updateScanCounter(int)));
    connect(_monitor, SIGNAL(hostFinisced(QStringList,QByteArray,QByteArray)),
            _parser, SLOT(startParser(QStringList,QByteArray,QByteArray)));

    // FIXME:: nse category -- port to nseManager
    connect(nseComboScript, SIGNAL(currentIndexChanged(int)),
            _nseManager, SLOT(updateNseOptionScript(int)));
    connect(nseActiveBut, SIGNAL(clicked()),
            _nseManager, SLOT(nseTreeActiveItem()));
    connect(nseRemoveBut, SIGNAL(clicked()),
            _nseManager, SLOT(nseTreeRemoveItem()));
    connect(nseResetBut, SIGNAL(clicked()),
            _nseManager, SLOT(nseTreeResetItem()));
    connect(nseTreeAvail, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            _nseManager, SLOT(requestNseHelp(QTreeWidgetItem*,int)));
    connect(nseTreeActive, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            _nseManager, SLOT(requestNseHelp(QTreeWidgetItem*,int)));
    connect(nseFixedSButt, SIGNAL(clicked()),
            this, SLOT(resetPar()));
    connect(searchButtHelp, SIGNAL(clicked()),
            _nseManager, SLOT(requestNseScriptHelp()));

    // action help menu (browser call)
    connect(actionReport_Bug, SIGNAL(triggered()),
            _utilities, SLOT(showBugUrl()));
    connect(actionVisit_Website, SIGNAL(triggered()),
            _utilities, SLOT(showHomepageUrl()));
    connect(actionDocumentation, SIGNAL(triggered()),
            _utilities, SLOT(showDocumentationUrl()));
    connect(actionDonate_Money, SIGNAL(triggered()),
            _utilities, SLOT(showDonateUrl()));

    // Discover
    connect(comboDiscover, SIGNAL(activated(QString)),
            this, SLOT(discoverIp(QString)));
    connect(startDiscoverButt, SIGNAL(clicked()),
            this, SLOT(discoverIpState()));
    connect(stopDiscoverButt, SIGNAL(clicked()),
            this, SLOT(stopDiscover()));
    connect(treeDiscover, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(runtimeScanDiscover()));
    connect(discoverScanButt, SIGNAL(clicked()),
            this, SLOT(callScanDiscover()));
}
