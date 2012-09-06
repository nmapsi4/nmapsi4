/***************************************************************************
 *   Copyright (C) 2010-2012 by Francesco Cecconi                          *
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

void MainWindow::syncSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");

    // TODO: load saved profile or simple default
    m_savePos = settings.value("savePos",true).toBool();
    m_saveSize = settings.value("saveSize",true).toBool();
    m_hostCache = settings.value("hostCache",10).toInt();

#if defined(Q_WS_WIN)
    // disable lookup in MS windows
    m_lookupType = 0;
#else
    m_lookupType = settings.value("lookupType", 1).toInt();
#endif

    // restore actionMenuBar
    actionMenuBar->setChecked(settings.value("showMenuBar", false).toBool());
    // update max parallel scan option
    m_monitor->updateMaxParallelScan();
}

void MainWindow::saveSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");

    if(m_savePos)
    {
        settings.setValue("window/pos", pos());
    }

    if(m_saveSize)
    {
        settings.setValue("window/size", size());
    }

    settings.setValue("savePos", m_savePos);
    settings.setValue("saveSize", m_saveSize);
    settings.setValue("hostCache",10);
    settings.setValue("splitterSizes", m_mainHorizontalSplitter->saveState());
    settings.setValue("splitterSizesRight", m_mainVerticalSplitter->saveState());
    settings.setValue("showMenuBar", actionMenuBar->isChecked());

    m_bookmark->syncSettings();
    m_discoverManager->syncSettings();
    m_parser->syncSettings();
    m_vulnerability->syncSettings();

// check and reset for settings file permission
#if !defined(Q_WS_WIN)
    if (!m_userId) {
        QFileInfo fileInfo;

        fileInfo.setFile(settings.fileName());
        if ((!fileInfo.permissions().testFlag(QFile::WriteOther) && !fileInfo.ownerId())) {
             QFile::setPermissions(settings.fileName(), QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
                QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }

        QSettings settingsBookmark("nmapsi4", "nmapsi4_bookmark");
        fileInfo.setFile(settingsBookmark.fileName());
        if ((!fileInfo.permissions().testFlag(QFile::WriteOther) && !fileInfo.ownerId())) {
            QFile::setPermissions(settingsBookmark.fileName(), QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
                QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }
    }
#endif
}

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
            m_hostModel = new QStringListModel(m_bookmark->getHostListFromBookmark(),this);
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
            m_vulnModel = new QStringListModel(m_bookmark->getServicesListFromBookmark(),this);
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

    // restore state of the QAction's connected to splitter widget
    if (!settings.value("splitterSizes").toByteArray().isEmpty())
    {
        m_mainHorizontalSplitter->restoreState(settings.value("splitterSizes").toByteArray());

        if (m_mainHorizontalSplitter->sizes()[0])
        {
            m_collections->m_collectionsButton.value("scan-list")->setChecked(true);
        }
        else
        {
            m_collections->m_collectionsButton.value("scan-list")->setChecked(false);
        }
    }
    else
    {
        m_collections->m_collectionsButton.value("scan-list")->setChecked(true);
    }

    if (!settings.value("splitterSizesRight").toByteArray().isEmpty())
    {
        m_mainVerticalSplitter->restoreState(settings.value("splitterSizesRight").toByteArray());

        if (m_mainVerticalSplitter->sizes()[1])
        {
            m_collections->m_collectionsButton.value("details-list")->setChecked(true);
        }
        else
        {
            m_collections->m_collectionsButton.value("details-list")->setChecked(false);
        }
    }
    else
    {
        m_collections->m_collectionsButton.value("details-list")->setChecked(true);
    }
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
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(true);

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
    m_mainHorizontalSplitter = new QSplitter(this);
    m_mainVerticalSplitter = new QSplitter(this);

    connect(m_mainVerticalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(resizeVerticalSplitterEvent()));

    connect(m_mainHorizontalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(resizeHorizontalSplitterEvent()));

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

