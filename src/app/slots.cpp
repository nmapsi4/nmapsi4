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

    connect(actionFullScreen, SIGNAL(triggered()),
            this, SLOT(setFullScreen()));
    connect(actionMenuBar, SIGNAL(triggered()),
            this, SLOT(updateMenuBar()));

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
    connect(monitorStopAllScanButt, SIGNAL(clicked()),
            m_monitor, SLOT(stopAllScan()));

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
