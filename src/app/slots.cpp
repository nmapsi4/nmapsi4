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
    // action slot
    connect(action_Quit, SIGNAL(triggered()),
            this, SLOT(exit()));    // file action menu
    connect(action_About, SIGNAL(triggered()),
            _utilities, SLOT(about()));    // about action menu
    connect(actionAbout_Qt, SIGNAL(triggered()),
            _utilities, SLOT(aboutQt()));    // about action menu
    connect(action_Scan_menu, SIGNAL(triggered()),
            this, SLOT(startScan()));    // about action menu
    connect(actionClear_History, SIGNAL(triggered()),
            this, SLOT(listClear()));
    connect(buttonHostClear, SIGNAL(clicked()),
            this, SLOT(clearHostnameCombo()));
    connect(buttonParametersClear, SIGNAL(clicked()),
            this, SLOT(clearParametersCombo()));
    connect(action_Scan_2, SIGNAL(triggered()),
            this, SLOT(startScan()));    // about action menu

    connect(actionProfile, SIGNAL(triggered()),
            this, SLOT(startProfile_ui()));
    connect(actionNew_Profile, SIGNAL(triggered()),
            this, SLOT(startProfilerManager()));
    connect(actionEdit_Profile, SIGNAL(triggered()),
            this, SLOT(editProfile()));

    connect(actionFullS, SIGNAL(triggered()),
            this, SLOT(checkFullScreen()));
    connect(actionMenuBar, SIGNAL(triggered()),
            this, SLOT(updateMenuBar()));
    connect(Bdetails, SIGNAL(clicked()),
            this, SLOT(detailsOptions()));
    connect(Bnss, SIGNAL(clicked()),
            this, SLOT(nssOptions()));
    connect(GItree, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuServiceMain()));

    // Obj parser runtime
    connect(treeMain, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            _parser, SLOT(showParserResult(QTreeWidgetItem*,int)));
    connect(treeTraceroot, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            _parser, SLOT(showParserTracerouteResult(QTreeWidgetItem*,int)));
    connect(actionSave, SIGNAL(triggered()),
            _parser, SLOT(callSaveSingleLogWriter()));
    connect(actionSave_Menu, SIGNAL(triggered()),
            _parser, SLOT(callSaveSingleLogWriter()));
    connect(actionSave_As_Menu, SIGNAL(triggered()),
            _parser, SLOT(callSaveAllLogWriter()));

    // Bookmarks signals
    connect(treeLogH, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuScanBook()));
    connect(treeBookVuln, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuVulnBook()));
    connect(treeBookPar, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuParBook()));

    connect(actionAdd_Bookmark, SIGNAL(triggered()),
            this, SLOT(saveBookMarks()));
    connect(actionAdd_Parameters_to_bookmark, SIGNAL(triggered()),
            this, SLOT(startAddParBook_ui()));


    // Vuln signal
    connect(treeWidgetVulnUrl, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(menuVulnUrlBookmark()));
    connect(comboPar, SIGNAL(activated(QString)),
            this, SLOT(slotParSelected()));
    connect(comboHostBook, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(slotHostSelected()));

    // main session
    connect(actionScan_section, SIGNAL(triggered()),
            this, SLOT(updateSezScan()));
    connect(actionVulnerabilities_section, SIGNAL(triggered()),
            this, SLOT(updateSezVuln()));
    connect(actionSection_Discover, SIGNAL(triggered()),
            this, SLOT(updateSezDiscover()));
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
    connect(monitorStopAllScanButt, SIGNAL(clicked()),
            _monitor, SLOT(stopAllScan()));

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
            _discoverManager, SLOT(discoverIp(QString)));
    connect(startDiscoverButt, SIGNAL(clicked()),
            _discoverManager, SLOT(discoverIpState()));
    connect(stopDiscoverButt, SIGNAL(clicked()),
            _discoverManager, SLOT(stopDiscover()));
    connect(treeDiscover, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            _discoverManager, SLOT(runtimeScanDiscover()));
    connect(discoverScanButt, SIGNAL(clicked()),
            _discoverManager, SLOT(callScanDiscover()));
    connect(reloadComboDiscover, SIGNAL(clicked()),
            _discoverManager, SLOT(startDiscover()));
}
