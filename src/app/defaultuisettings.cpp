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

#include "mainwindow.h"

void MainWindow::updateCompleter()
{
    if (!m_bookmark->isBookmarkHostListEmpty())
    {
        if (!m_completer.isNull())
        {
            QStringListModel *newModel = qobject_cast<QStringListModel*>(m_completer.data()->model());
            newModel->setStringList(m_bookmark->getHostListFromBookmark());
        }
        else if (m_hostModel.isNull())
        {
            m_hostModel = new QStringListModel(m_bookmark->getHostListFromBookmark());
        }
    }

    if (!m_bookmark->isBookmarkServicesListEmpty())
    {
        if (!m_completerVuln.isNull())
        {
            QStringListModel *newModel = qobject_cast<QStringListModel*>(m_completerVuln.data()->model());
            newModel->setStringList(m_bookmark->getServicesListFromBookmark());
        }
        else if (m_vulnModel.isNull())
        {
            m_vulnModel = new QStringListModel(m_bookmark->getServicesListFromBookmark());
        }
    }
}

void MainWindow::restoreSettings()
{
    // restore window position
    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("window/size", QSize(910, 672)).toSize();
    resize(size);
    move(pos);
    m_mainHorizontalSplitter->restoreState(settings.value("splitterSizes").toByteArray());
    m_mainVerticalSplitter->restoreState(settings.value("splitterSizesRight").toByteArray());
}

void MainWindow::setTreeSettings()
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

void MainWindow::setDefaultAction()
{
    // Set default properties
    action_Scan_menu->setEnabled(false);
    action_Scan_2->setEnabled(false);
    actionAdd_Bookmark->setEnabled(false);
    toolBarSearch->setVisible(false);
    m_collectionsButton.value("scan-sez")->setChecked(true);

    Bdetails->setChecked(true);
    Bdetails->setStyleSheet(verticalStyleSheet);
    Bnss->setStyleSheet(verticalStyleSheet);

    menuBar()->setContextMenuPolicy(Qt::PreventContextMenu);
    nseNumber->setSegmentStyle(QLCDNumber::Flat);
    tWresult->setVisible(false);

    tabUi->removeTab(tabUi->indexOf(tabVulnMain));
    tabUi->removeTab(tabUi->indexOf(tabVulnBookmarks));
    tabUi->removeTab(tabUi->indexOf(tabDiscover));

    actionAdd_vulnerability_search_url->setVisible(false);
}

void MainWindow::setDefaultSplitter()
{
    // define default Ui splitter
    m_mainHorizontalSplitter = new QSplitter();
    m_mainVerticalSplitter = new QSplitter();
    m_mainHorizontalSplitter->setOrientation(Qt::Horizontal);
    m_mainHorizontalSplitter->addWidget(frameLeft);
    m_mainHorizontalSplitter->addWidget(frameCenter);
    //frameCenter
    m_mainVerticalSplitter->setOrientation(Qt::Vertical);
    m_mainVerticalSplitter->addWidget(tabWidget);
    m_mainVerticalSplitter->addWidget(frameRight);
    // insert splitter
    tabUi->widget(0)->layout()->addWidget(m_mainHorizontalSplitter);
    frameCenter->layout()->addWidget(m_mainVerticalSplitter);
    frameCenter->layout()->addWidget(frame_2);
}

void MainWindow::defaultComboValues()
{
    hostEdit->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    hostEdit->insertItem(0, tr("Host(s) to scan (ip/dns or incremental - ex. 192.168.1.1/20)"));
    comboVulnRis->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboVulnRis->insertItem(0, tr("Search Vulnerabilities"));
}
