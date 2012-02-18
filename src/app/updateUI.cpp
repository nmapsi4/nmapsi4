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

void MainWindow::detailsOptions()
{
    Bdetails->setChecked(true);
    Bnss->setChecked(false);
    SWscan->setCurrentIndex(0);
    updateIconsBox();
}

void MainWindow::nssOptions()
{
    Bdetails->setChecked(false);
    Bnss->setChecked(true);
    SWscan->setCurrentIndex(1);
    tabWidget->setCurrentIndex(0);
    updateIconsBox();
}

void MainWindow::checkFullScreen()
{

    if (isFullScreen())
    {
        setWindowState(windowState() & ~Qt::WindowFullScreen );
        actionFullS->setChecked(false);
    }
    else
    {
        setWindowState(windowState() | Qt::WindowFullScreen );
        actionFullS->setChecked(true);
    }
}

void MainWindow::updateMenuBar()
{
    if (actionMenuBar->isChecked())
    {
        menuBar()->setVisible(true);
    }
    else
    {
        menuBar()->setVisible(false);
    }
}

void MainWindow::updateIconsBox()
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
    }
}

void MainWindow::updateComboHostnameProperties()
{
    action_Scan_menu->setEnabled(true);
    action_Scan_2->setEnabled(true);
    hostEdit->clear();
    hostEdit->setStyleSheet(QString::fromUtf8(""));
    bool signalState = hostEdit->lineEdit()->disconnect(SIGNAL(cursorPositionChanged(int,int)));

    if (!signalState)
	return;

    connect(hostEdit, SIGNAL(editTextChanged(QString)),
            this, SLOT(linkCompleterToHostname()));
}

void MainWindow::updateSezScan()
{ // SLOT
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(true);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(false);
    toolBar->setVisible(true);
    toolBar_2->setVisible(true);
    toolBarSearch->setVisible(false);
    toolBarBook->setVisible(true);

    if (!tabScan->isVisible())
    {
        tabUi->insertTab(0,tabScan,QIcon(QString::fromUtf8(":/images/images/network_local.png")),"Scan");
        tabUi->setCurrentIndex(0);
    }

    if (!bookmarks->isVisible())
    {
        tabUi->insertTab(tabUi->count(),bookmarks,tr("Bookmarks"));
        tabUi->setTabIcon(tabUi->indexOf(bookmarks),QIcon(QString::fromUtf8(":/images/images/bookmark_folder.png")));
    }

    tabUi->setTabIcon(0,QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    tabUi->setTabText(0, "Scan");

    if (!tabLog->isVisible())
    {
        tabUi->insertTab(tabUi->count(),tabLog,QIcon(QString::fromUtf8(":/images/images/book.png")),"Raw scan log");
    }

    tabUi->removeTab(tabUi->indexOf(tabVulnMain));
    tabUi->removeTab(tabUi->indexOf(tabDiscover));
    tabUi->removeTab(tabUi->indexOf(tabVulnBookmarks));
    // enable scan action
    toolBarTab->setVisible(true);

    menu_Bookmaks->setEnabled(true);
    actionAdd_Bookmark->setText(tr("&Add host to bookmark"));
    actionAdd_Parameters_to_bookmark->setVisible(true);
    actionAdd_vulnerability_search_url->setVisible(false);
}

void MainWindow::updateSezVuln()
{ // SLOT
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(true);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(false);
    // main and action bar only in scan index
    toolBar->setVisible(false);
    toolBarBook->setVisible(true);
    toolBar_2->setVisible(false);
    toolBarSearch->setVisible(true);

    tabUi->removeTab(tabUi->indexOf(bookmarks));
    tabUi->removeTab(tabUi->indexOf(tabScan));
    tabUi->removeTab(tabUi->indexOf(tabLog));
    tabUi->removeTab(tabUi->indexOf(tabDiscover));
    tabUi->insertTab(0,tabVulnMain,QIcon(QString::fromUtf8(":/images/images/viewmag+.png")),"Vulnerability");

    if (!tabVulnBookmarks->isVisible())
    {
        tabUi->insertTab(tabUi->count(),tabVulnBookmarks,tr("Bookmarks"));
        tabUi->setTabIcon(tabUi->indexOf(tabVulnBookmarks),QIcon(QString::fromUtf8(":/images/images/bookmark_folder.png")));
    }

    tabUi->setCurrentIndex(0);

    // disable scan action
    toolBarTab->setVisible(false);

    menu_Bookmaks->setEnabled(true);
    actionAdd_Bookmark->setText(tr("Add service to &bookmark"));
    actionAdd_Parameters_to_bookmark->setVisible(false);
    actionAdd_vulnerability_search_url->setVisible(true);
}

void MainWindow::updateSezDiscover()
{ // SLOT
    // discover section
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(true);
    // main and action bar only in scan index
    toolBar->setVisible(false);
    toolBarBook->setVisible(false);
    toolBar_2->setVisible(false);
    toolBarSearch->setVisible(false);

    tabUi->removeTab(tabUi->indexOf(bookmarks));
    tabUi->removeTab(tabUi->indexOf(tabScan));
    tabUi->removeTab(tabUi->indexOf(tabLog));
    tabUi->removeTab(tabUi->indexOf(tabVulnMain));
    tabUi->removeTab(tabUi->indexOf(tabVulnBookmarks));
    tabUi->insertTab(0,tabDiscover,QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")),"Network discover");
    tabUi->setCurrentIndex(0);

    // disable scan action
    toolBarTab->setVisible(false);

    menu_Bookmaks->setEnabled(false);
}

void MainWindow::setTreeWidgetValues()
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

void MainWindow::loadDefaultProfile()
{
    comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboAdv->insertItem(0, loadExtensions().join(" "));
}

void MainWindow::updateComboBook()
{
    comboHostBook->clear();
    comboHostBook->insertItem(0, "Select Saved Host");

    for(int index=0; index < treeLogH->topLevelItemCount(); index++)
    {
        comboHostBook->insertItem(1, treeLogH->topLevelItem(index)->text(0));
    }
}

void MainWindow::clearAll()
{
    // Host list
    m_parser->clearParserItems();
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

void MainWindow::updateScanCounter(int hostNumber)
{
    if (hostNumber == 1)
    {
        tabUi->setTabIcon(tabUi->indexOf(tabMainMonitor),QIcon(QString::fromUtf8(":/images/images/reload.png")));
    }

    QString title("Nmapsi4 -" + tr(" Active Scan ")
                  + QLatin1String("(")
                  + QString("%1").arg(m_monitor->monitorHostNumber())
                  + QLatin1String(")"));

    setWindowTitle(title);
}

void MainWindow::monitorRuntimeEvent()
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

void MainWindow::clearHostnameCombo()
{
    // reset combo host bookmarks to default value
    comboHostBook->setCurrentIndex(0);
    hostEdit->clearEditText();
}

void MainWindow::clearParametersCombo()
{
    // TODO:set to default saved parameters profile
    comboPar->setCurrentIndex(0);
    comboParametersSelectedEvent();
}
