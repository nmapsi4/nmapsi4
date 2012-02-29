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

#include "mainwindow.h"

void MainWindow::connectSlots()
{
    // action slot
    connect(action_Quit, SIGNAL(triggered()),
            this, SLOT(close()));    // file action menu
    connect(action_About, SIGNAL(triggered()),
            m_utilities, SLOT(about()));    // about action menu
    connect(actionAbout_Qt, SIGNAL(triggered()),
            m_utilities, SLOT(aboutQt()));    // about action menu
    connect(action_Scan_menu, SIGNAL(triggered()),
            this, SLOT(startScan()));    // about action menu
    connect(actionClear_History, SIGNAL(triggered()),
            this, SLOT(clearAll()));
    connect(buttonHostClear, SIGNAL(clicked()),
            this, SLOT(clearHostnameCombo()));
    connect(buttonParametersClear, SIGNAL(clicked()),
            this, SLOT(clearParametersCombo()));
    connect(action_Scan_2, SIGNAL(triggered()),
            this, SLOT(startScan()));    // about action menu

    connect(actionProfile, SIGNAL(triggered()),
            this, SLOT(startPreferencesDialog()));
    connect(actionNew_Profile, SIGNAL(triggered()),
            this, SLOT(newProfile()));
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

    // Obj parser runtime
    connect(treeMain, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            m_parser, SLOT(showParserResult(QTreeWidgetItem*,int)));
    connect(treeTraceroot, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
            m_parser, SLOT(showParserTracerouteResult(QTreeWidgetItem*,int)));
    connect(actionSave, SIGNAL(triggered()),
            m_parser, SLOT(callSaveSingleLogWriter()));
    connect(actionSave_Menu, SIGNAL(triggered()),
            m_parser, SLOT(callSaveSingleLogWriter()));
    connect(actionSave_As_Menu, SIGNAL(triggered()),
            m_parser, SLOT(callSaveAllLogWriter()));

    // ActionManager signals
    connect(treeLogH, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            m_collections, SLOT(scanBookmarkContextMenu()));
    connect(treeBookPar, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            m_collections, SLOT(parametersBookmarkContextMenu()));
    connect(GItree, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            m_collections, SLOT(mainServicesContextMenu()));
    connect(treeBookVuln, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            m_collections, SLOT(servicesContextMenu()));
    connect(treeWidgetVulnUrl, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            m_collections, SLOT(vulnerabilityUrlContextMenu()));


    connect(actionAdd_Bookmark, SIGNAL(triggered()),
            m_bookmark, SLOT(saveItemToBookmarks()));
    connect(actionAdd_Parameters_to_bookmark, SIGNAL(triggered()),
            m_bookmark, SLOT(startParametersToBookmarksDialog()));


    // Vuln signal
    connect(comboPar, SIGNAL(activated(QString)),
            this, SLOT(comboParametersSelectedEvent()));
    connect(comboHostBook, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(quickAddressSelectionEvent()));

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
            this, SLOT(updateComboHostnameProperties()));

    // monitor events
    connect(scanMonitor, SIGNAL(itemSelectionChanged()),
            this, SLOT(monitorRuntimeEvent()));
    connect(monitorStopCurrentScanButt, SIGNAL(clicked()),
            m_monitor, SLOT(stopSelectedScan()));
    connect(monitorDetailsScanButt, SIGNAL(clicked()),
            m_monitor, SLOT(showSelectedScanDetails()));
    connect(m_monitor, SIGNAL(monitorUpdated(int)),
            this, SLOT(updateScanCounter(int)));
    connect(m_monitor, SIGNAL(hostFinisced(QStringList,QByteArray,QByteArray)),
            m_parser, SLOT(startParser(QStringList,QByteArray,QByteArray)));
    connect(monitorStopAllScanButt, SIGNAL(clicked()),
            m_monitor, SLOT(stopAllScan()));

    // action help menu (browser call)
    connect(actionReport_Bug, SIGNAL(triggered()),
            m_utilities, SLOT(showBugUrl()));
    connect(actionVisit_Website, SIGNAL(triggered()),
            m_utilities, SLOT(showHomepageUrl()));
    connect(actionDocumentation, SIGNAL(triggered()),
            m_utilities, SLOT(showDocumentationUrl()));
    connect(actionDonate_Money, SIGNAL(triggered()),
            m_utilities, SLOT(showDonateUrl()));

    // Discover
    connect(comboDiscover, SIGNAL(activated(QString)),
            m_discoverManager, SLOT(discoverIp(QString)));
    connect(startDiscoverButt, SIGNAL(clicked()),
            m_discoverManager, SLOT(startDiscoverIpsFromRange()));
    connect(cidrButton, SIGNAL(clicked()),
            m_discoverManager, SLOT(startDiscoverIpsFromCIDR()));
    connect(stopDiscoverButt, SIGNAL(clicked()),
            m_discoverManager, SLOT(stopDiscoverFromIpsRange()));
    connect(stopDiscoverCidrButton, SIGNAL(clicked()),
            m_discoverManager, SLOT(stopDiscoverFromCIDR()));
    connect(treeDiscover, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            m_discoverManager, SLOT(runtimeScanDiscover()));

    connect(reloadComboDiscover, SIGNAL(clicked()),
            m_discoverManager, SLOT(startDiscover()));
    connect(calculateAddressButton, SIGNAL(clicked()),
            m_discoverManager, SLOT(calculateAddressFromCIDR()));
}
