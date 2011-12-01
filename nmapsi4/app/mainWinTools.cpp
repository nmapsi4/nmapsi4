/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
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

void nmapClass::loadHistoryDefault()
{
    // reload bookmark from history
    logHistory *historyScan_ = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
    historyScan_->updateBookMarks();
    delete historyScan_;
    // check for user or admin parameters bookmarks
    if (!uid) 
    {
        logHistory *historyPar_ = new logHistory(treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
        historyPar_->updateBookMarks();
        delete historyPar_;
    } 
    else 
    {
        logHistory *historyPar_ = new logHistory(treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
        historyPar_->updateBookMarks();
        delete historyPar_;
    }

    logHistory *historyVuln_ = new logHistory(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
    historyVuln_->updateBookMarks();
    delete historyVuln_;
    
    logHistory *historyVulnUrl_ = new logHistory(treeWidgetVulnUrl, "nmapsi4/nameUrlVuln", "nmapsi4/nameUrlAddr", -1);
    historyVulnUrl_->updateBookMarks();
    delete historyVulnUrl_;
}

void nmapClass::restoreSettings()
{
    // restore window position
    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("window/size", QSize(910, 672)).toSize();
    resize(size);
    move(pos);
    cW->restoreState(settings.value("splitterSizes").toByteArray());
    bW->restoreState(settings.value("splitterSizesRight").toByteArray());
}

void nmapClass::setTreeSettings()
{
    // set TreeWidget properties
    treeLogH->setColumnWidth(0, 400);
    treeBookPar->setColumnWidth(0, 400);
    scanMonitor->setColumnWidth(0, 300);
    scanMonitor->setColumnWidth(1, 350);
    //treeTraceroot->setColumnWidth(0, 250);
    treeTraceroot->setColumnWidth(1, 100);
    treeTraceroot->setColumnWidth(2, 200);
    treeTraceroot->setColumnWidth(3, 200);
    treeBookVuln->setColumnWidth(0, 400);
    treeWidgetVulnUrl->setColumnWidth(0, 400);
    treeMain->setColumnWidth(0, 200);
    treeDiscover->setColumnWidth(0, 300);
}

void nmapClass::setDefaultAction()
{
    // Set default properties
    action_Scan_menu->setEnabled(false);
    action_Scan_2->setEnabled(false);
    hostEdit->setEnabled(false);
    actionAdd_Bookmark->setEnabled(false);
    action_Add_BookmarkToolBar->setEnabled(false);
    toolBarSearch->setVisible(false);
    _collectionsButton.value("scan-sez")->setChecked(true);
    Bdetails->setChecked(true);
    menuBar()->setContextMenuPolicy(Qt::PreventContextMenu);
    nseNumber->setSegmentStyle(QLCDNumber::Flat);
    tWresult->setVisible(false);
    tabUi->removeTab(tabUi->indexOf(tabLog));
    tabUi->removeTab(tabUi->indexOf(tabVulnMain));
    tabUi->removeTab(tabUi->indexOf(tabDiscover));
}

void nmapClass::setDefaultSplitter()
{
    // define default Ui splitter
    cW = new QSplitter();
    bW = new QSplitter();    
    cW->setOrientation(Qt::Horizontal);
    cW->addWidget(frameLeft);
    cW->addWidget(frameCenter);
    //frameCenter
    bW->setOrientation(Qt::Vertical);
    bW->addWidget(tabWidget);
    bW->addWidget(frameRight);
    // insert splitter
    tabUi->widget(0)->layout()->addWidget(cW);
    frameCenter->layout()->addWidget(bW);
    frameCenter->layout()->addWidget(frame_2);
}

void nmapClass::defaultComboValues()
{
    hostEdit->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    hostEdit->insertItem(0, tr("Insert [ip] or [dns] or [ip range] or [ip/dns list with space separator] to scan (ip range ex. 192.168.1.10/20)"));
    comboVulnRis->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboVulnRis->insertItem(0, tr("Search Vulnerabilities"));
}
