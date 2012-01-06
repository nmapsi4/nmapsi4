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

void nmapClass::detailsOptions()
{
    Bdetails->setChecked(true);
    Bnss->setChecked(false);
    Bbook->setChecked(false);
    BBPar->setChecked(false);
    SWscan->setCurrentIndex(0);
    updateIconsBox();
}

void nmapClass::nssOptions()
{
    Bdetails->setChecked(false);
    Bnss->setChecked(true);
    Bbook->setChecked(false);
    BBPar->setChecked(false);
    SWscan->setCurrentIndex(1);
    tabWidget->setCurrentIndex(0);
    updateIconsBox();
}

void nmapClass::bookOptions()
{
    Bdetails->setChecked(false);
    Bnss->setChecked(false);
    Bbook->setChecked(true);
    BBPar->setChecked(false);
    SWscan->setCurrentIndex(2);
    updateIconsBox();
}

void nmapClass::bookOptionsPar()
{
    Bdetails->setChecked(false);
    Bnss->setChecked(false);
    Bbook->setChecked(false);
    BBPar->setChecked(true);
    SWscan->setCurrentIndex(3);
    updateIconsBox();
}

void nmapClass::updateIconsBox()
{
    int tmpBox = SWscan->currentIndex();
    switch (tmpBox)
    {
    case 0:
        Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
	break;
    case 1:
        Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
        break;
    case 2:
        Bbook->setIcon(QIcon(QString::fromUtf8(":/images/images/bookmark_folder.png")));
	break;
    case 3:
        BBPar->setIcon(QIcon(QString::fromUtf8(":/images/images/bookmark_folder.png")));
	break;
    }
}

void nmapClass::updateFontHost()
{
    action_Scan_menu->setEnabled(true);
    action_Scan_2->setEnabled(true);
    hostEdit->clear();
    hostEdit->setStyleSheet(QString::fromUtf8(""));
    bool signalState = hostEdit->lineEdit()->disconnect(SIGNAL(cursorPositionChanged(int,int)));

    if (!signalState)
	return;

    connect(hostEdit, SIGNAL(editTextChanged(QString)),
            this, SLOT(callSearchHistory()));
}

void nmapClass::updateSezScan()
{ // SLOT
    _collectionsButton.value("scan-sez")->setChecked(true);
    _collectionsButton.value("log-sez")->setChecked(false);
    _collectionsButton.value("vuln-sez")->setChecked(false);
    _collectionsButton.value("discover-sez")->setChecked(false);
    toolBar->setVisible(true);
    toolBar_2->setVisible(true);
    toolBarSearch->setVisible(false);
    toolBarBook->setVisible(true);

    if (!tabScan->isVisible())
    {
        tabUi->insertTab(0,tabScan,QIcon(QString::fromUtf8(":/images/images/network_local.png")),"Scan");
        tabUi->setCurrentIndex(0);
    }

    if (!tabNseOpt->isVisible())
    {
        tabUi->insertTab(tabUi->count(),tabNseOpt,tr("Nse Options"));
        tabUi->setTabIcon(tabUi->indexOf(tabNseOpt),QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    }
    tabUi->setTabIcon(0,QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    tabUi->setTabText(0, "Scan");

    tabUi->removeTab(tabUi->indexOf(tabVulnMain));
    tabUi->removeTab(tabUi->indexOf(tabLog));
    tabUi->removeTab(tabUi->indexOf(tabDiscover));
    // enable scan action
    toolBarTab->setVisible(true);
}

void nmapClass::updateSezLog()
{  // SLOT
    _collectionsButton.value("scan-sez")->setChecked(false);
    _collectionsButton.value("log-sez")->setChecked(true);
    _collectionsButton.value("vuln-sez")->setChecked(false);
    _collectionsButton.value("discover-sez")->setChecked(false);
    // main and action bar only in scan index
    toolBar->setVisible(false);
    toolBar_2->setVisible(false);
    toolBarBook->setVisible(false);
    toolBarSearch->setVisible(false);

    tabUi->removeTab(tabUi->indexOf(tabNseOpt));
    tabUi->removeTab(tabUi->indexOf(tabScan));
    tabUi->removeTab(tabUi->indexOf(tabVulnMain));
    tabUi->removeTab(tabUi->indexOf(tabDiscover));
    tabUi->insertTab(0,tabLog,QIcon(QString::fromUtf8(":/images/images/book.png")),"Scan Log");
    tabUi->setCurrentIndex(0);

    // disable scan action
    toolBarTab->setVisible(false);
}

void nmapClass::updateSezVuln()
{ // SLOT
    _collectionsButton.value("scan-sez")->setChecked(false);
    _collectionsButton.value("log-sez")->setChecked(false);
    _collectionsButton.value("vuln-sez")->setChecked(true);
    _collectionsButton.value("discover-sez")->setChecked(false);
    // main and action bar only in scan index
    toolBar->setVisible(false);
    toolBarBook->setVisible(true);
    toolBar_2->setVisible(false);
    toolBarSearch->setVisible(true);

    tabUi->removeTab(tabUi->indexOf(tabNseOpt));
    tabUi->removeTab(tabUi->indexOf(tabScan));
    tabUi->removeTab(tabUi->indexOf(tabLog));
    tabUi->removeTab(tabUi->indexOf(tabDiscover));
    tabUi->insertTab(0,tabVulnMain,QIcon(QString::fromUtf8(":/images/images/viewmag+.png")),"Vulnerability");
    tabUi->setCurrentIndex(0);

    // disable scan action
    toolBarTab->setVisible(false);
}

void nmapClass::updateDiscoverVuln()
{ // SLOT
    // discover section
    _collectionsButton.value("scan-sez")->setChecked(false);
    _collectionsButton.value("log-sez")->setChecked(false);
    _collectionsButton.value("vuln-sez")->setChecked(false);
    _collectionsButton.value("discover-sez")->setChecked(true);
    // main and action bar only in scan index
    toolBar->setVisible(false);
    toolBarBook->setVisible(false);
    toolBar_2->setVisible(false);
    toolBarSearch->setVisible(false);

    tabUi->removeTab(tabUi->indexOf(tabNseOpt));
    tabUi->removeTab(tabUi->indexOf(tabScan));
    tabUi->removeTab(tabUi->indexOf(tabLog));
    tabUi->removeTab(tabUi->indexOf(tabVulnMain));
    tabUi->insertTab(0,tabDiscover,QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")),"Discover");
    tabUi->setCurrentIndex(0);

    // disable scan action
    toolBarTab->setVisible(false);
}

void nmapClass::NSSCheck()
{ // SLOT
   if (_collectionsButton.value("nss-act")->isChecked())
   {
       NSSsupport = true;
       Bnss->setVisible(true);
       resetPar();
   }
   else
   {
       // check for nse combo
       nseComboScript->setCurrentIndex(0);
       NSSsupport = false;
       Bnss->setVisible(false);
       resetPar();
   }
}

void nmapClass::setTreeWidgetValues()
{
    listWscan->setIconSize(QSize(22, 22));
    listWscan->header()->setResizeMode(0, QHeaderView::Interactive);
    listScanError->setIconSize(QSize(22, 22));
    listScanError->header()->setResizeMode(0, QHeaderView::Interactive);
    listScan->setIconSize(QSize(22, 22));
    listScan->header()->setResizeMode(0, QHeaderView::Interactive);
    treeTraceroot->setIconSize(QSize(22, 22));
    treeTraceroot->header()->setResizeMode(0, QHeaderView::Interactive);
    treeNSS->setIconSize(QSize(22, 22));
    treeNSS->header()->setResizeMode(0, QHeaderView::Interactive);
    treeMain->setIconSize(QSize(22, 22));
    treeMain->header()->setResizeMode(0, QHeaderView::Interactive);
    treeLookup->setIconSize(QSize(22, 22));
    treeLookup->header()->setResizeMode(0, QHeaderView::Interactive);
    scanMonitor->setIconSize(QSize(22, 22));
    scanMonitor->header()->setResizeMode(0, QHeaderView::Interactive);
}

void nmapClass::loadDefaultProfile()
{
    comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboAdv->insertItem(0, loadExtensions().join(" "));
}

void nmapClass::updateComboBook()
{
    comboHostBook->clear();
    comboHostBook->insertItem(0, "Select Saved Host");

    for(int index=0; index < treeLogH->topLevelItemCount(); index++)
    {
        comboHostBook->insertItem(1, treeLogH->topLevelItem(index)->text(0));
    }
}

void nmapClass::updateTabLook()
{
    if (_collectionsButton.value("tab-look-act")->isChecked())
    {
        tabWidget->insertTab(tabWidget->count(),tab_3,tr("Lookup"));
        tabWidget->setTabIcon(tabWidget->indexOf(tab_3),QIcon(QString::fromUtf8(":/images/images/network-workgroup.png")));
        LookupEnabled = true;
    }
    else
    {
        LookupEnabled = false;
        tabWidget->removeTab(tabWidget->indexOf(tab_3));
    }
}

void nmapClass::updateTabTrace()
{
    if (_collectionsButton.value("tab-trace-act")->isChecked())
    {
        tabWidget->insertTab(tabWidget->count(),tab_7,tr("Traceroute"));
        tabWidget->setTabIcon(tabWidget->indexOf(tab_7),QIcon(QString::fromUtf8(":/images/images/network-wired.png")));
        TraceEnabled = true;
    }
    else
    {
        TraceEnabled = false;
        tabWidget->removeTab(tabWidget->indexOf(tab_7));
    }
}

void nmapClass::listClear()
{
    // Host list
    _parser->clearParserItems();
    listScanError->clear();
    treeMain->clear();
    treeLookup->clear();
    treeTraceroot->clear();
    treeHostDet->clear();
    GItree->clear();
    listWscan->clear();
    treeNSS->clear();
    listScan->clear();
    actionClear_History->setEnabled(false);

    actionSave->setEnabled(false);
    actionSave_Menu->setEnabled(false);
    actionSave_As_Menu->setEnabled(false);

    comboScanLog->clear();
    comboScanLog->addItem(tr("Scan log parameters"));
}

void nmapClass::updateScanCounter(int hostNumber)
{
    if (hostNumber == 1)
    {
        tabUi->setTabIcon(tabUi->indexOf(tabMainMonitor),QIcon(QString::fromUtf8(":/images/images/reload.png")));
    }

    QString title("Nmapsi4 -" + tr(" Active Scan ")
                  + QLatin1String("(")
                  + QString("%1").arg(_monitor->monitorHostNumber())
                  + QLatin1String(")"));

    setWindowTitle(title);
}

void nmapClass::monitorRuntimeEvent()
{
    if (!monitorStopCurrentScanButt->isEnabled())
    {
        monitorStopCurrentScanButt->setEnabled(true);
    }

    if (!monitorDetailsScanButt->isEnabled())
    {
        monitorDetailsScanButt->setEnabled(true);
    }
}
