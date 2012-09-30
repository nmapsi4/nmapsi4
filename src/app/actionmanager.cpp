/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
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

#include "actionmanager.h"
#include "mainwindow.h"

ActionManager::ActionManager(MainWindow* parent)
    : QObject(parent), m_ui(parent)
{
    m_bottomUiToggleBar = new QToolBar(m_ui);
    m_bottomSectionBar = new QToolBar(m_ui);

    m_bottomUiToggleBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_bottomUiToggleBar->setStyleSheet("QToolBar { border: 0px; }");
    m_bottomUiToggleBar->setFloatable(false);
    m_bottomUiToggleBar->setMovable(false);

    m_bottomSectionBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_bottomSectionBar->setStyleSheet("QToolBar { border: 0px; }");
    m_bottomSectionBar->setFloatable(false);
    m_bottomSectionBar->setMovable(false);

    m_ui->addToolBar(Qt::BottomToolBarArea, m_bottomSectionBar);
    m_ui->addToolBar(Qt::BottomToolBarArea, m_bottomUiToggleBar);


    // ActionManager signals
    connect(m_ui->m_bookmark->m_scanBookmarkWidget->treeLogH, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(scanBookmarkContextMenu()));
    connect(m_ui->m_bookmark->m_scanBookmarkWidget->treeBookPar, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(parametersBookmarkContextMenu()));
    connect(m_ui->m_scanWidget->GItree, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(mainServicesContextMenu()));
    connect(m_ui->m_bookmark->m_vulnBookmarkWidget->treeBookVuln, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(servicesContextMenu()));
    connect(m_ui->m_bookmark->m_vulnBookmarkWidget->treeWidgetVulnUrl, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(vulnerabilityUrlContextMenu()));
}

ActionManager::~ActionManager()
{
}

void ActionManager::createSectionsBar()
{
    PushButtonOrientated* actionButt;

    actionButt = new PushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/network_local.png")), tr("Scan host(s)"), m_ui);
    actionButt->setToolTip(tr("Scan host(s)"));
    //actionButt->setFlat(true);
    actionButt->setCheckable(true);
    m_collectionsButton.insert("scan-sez", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(updateSezScan()));
    m_bottomSectionBar->addWidget(actionButt);

    actionButt = new PushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")), tr("Network Discover"), m_ui);
    actionButt->setToolTip(tr("Network Discover"));
    //actionButt->setFlat(true);
    actionButt->setCheckable(true);
    m_collectionsButton.insert("discover-sez", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(updateSezDiscover()));
    m_bottomSectionBar->addWidget(actionButt);

    actionButt = new PushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")), tr("Vulnerability"), m_ui);
    actionButt->setToolTip(tr("Check Vulnerabilities"));
    //actionButt->setFlat(true);
    actionButt->setCheckable(true);
    m_collectionsButton.insert("vuln-sez", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(updateSezVuln()));
    m_bottomSectionBar->addWidget(actionButt);

    m_bottomSectionBar->addSeparator();

    actionButt = new PushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/view-restore.png")), tr("Show/Hide host list"), m_ui);
    actionButt->setToolTip(tr("Show/Hide Host List"));
    actionButt->setFlat(true);
    actionButt->setCheckable(true);
    m_collectionsButton.insert("scan-list", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(resizeScanListWidgetEvent()));
    m_bottomUiToggleBar->addWidget(actionButt);

    actionButt = new PushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/view-restore.png")), tr("Show/Hide host details"), m_ui);
    actionButt->setToolTip(tr("Show/Hide Host Details"));
    actionButt->setFlat(true);
    actionButt->setCheckable(true);
    m_collectionsButton.insert("details-list", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(resizeHostDetailsWidgetEvent()));
    m_bottomUiToggleBar->addWidget(actionButt);
}

void ActionManager::scanBookmarkContextMenu()
{
    QAction removeBook(m_ui);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Host"));

    QAction scanBook(m_ui);
    scanBook.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    scanBook.setIconText(tr("Scan Host"));

    connect(&scanBook, SIGNAL(triggered()), m_ui, SLOT(takeHostFromBookmark()));
    connect(&removeBook, SIGNAL(triggered()), m_ui->m_bookmark, SLOT(deleteItemFromBookmark()));

    QMenu menuBook(m_ui);
    menuBook.addAction(&scanBook);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}

void ActionManager::parametersBookmarkContextMenu()
{
    QAction removeBook(m_ui);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Scan Parameters"));

    connect(&removeBook, SIGNAL(triggered()), m_ui->m_bookmark, SLOT(deleteParametersFromBookmark()));

    QMenu menuBook(m_ui);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}

void ActionManager::servicesContextMenu()
{
    QAction removeBook(m_ui);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Service"));

    QAction addBook(m_ui);
    addBook.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    addBook.setIconText(tr("Search for vulnerabilities"));

    connect(&addBook, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(callVulnCheck()));
    connect(&removeBook, SIGNAL(triggered()), m_ui->m_bookmark, SLOT(deleteItemFromBookmark()));

    QMenu menuBook(m_ui);
    menuBook.addAction(&addBook);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}


void ActionManager::mainServicesContextMenu()
{
    QAction checkVuln(m_ui);
    checkVuln.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    checkVuln.setIconText(tr("Check Vulnerability"));

    connect(&checkVuln, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(objVulnButton()));

    QMenu menuVulnMain(m_ui);
    menuVulnMain.addAction(&checkVuln);
    menuVulnMain.exec(QCursor::pos());
}

void ActionManager::vulnerabilityUrlContextMenu()
{
    QAction removeVulnUrl(m_ui);
    removeVulnUrl.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeVulnUrl.setIconText(tr("Remove url"));
    connect(&removeVulnUrl, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(removeUrlToBookmarks()));

    QMenu menuVulnUrl(m_ui);
    menuVulnUrl.addAction(&removeVulnUrl);
    menuVulnUrl.exec(QCursor::pos());
}

void ActionManager::createToolButtonBar()
{
    // new QToolButton menu
    m_menuSetup = new QToolButton(m_ui);
    m_menuSetup->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_menuSetup->setPopupMode(QToolButton::InstantPopup);
    m_menuSetup->setText(tr("Control"));
    m_menuSetup->setIcon(QIcon::fromTheme("applications-system", QIcon(":/images/images/applications-system.png")));

    QMenu *menu = new QMenu(m_ui);
    menu->addAction(m_ui->action_Scan_2);
    menu->addAction(m_ui->actionSave);
    menu->addAction(m_ui->actionSave_As_Menu);
    menu->addSeparator();
    menu->addMenu(m_ui->menu_Bookmaks);
    menu->addSeparator();
    menu->addAction(m_ui->actionScan_section);
    menu->addAction(m_ui->actionVulnerabilities_section);
    menu->addAction(m_ui->actionSection_Discover);
    menu->addSeparator();
    menu->addAction(m_ui->actionProfile);
    menu->addAction(m_ui->actionNew_Profile);
    menu->addAction(m_ui->actionEdit_Profile);
    menu->addSeparator();
    menu->addMenu(m_ui->menu_Help);
    menu->addSeparator();
    menu->addAction(m_ui->actionMenuBar);
    menu->addSeparator();
    menu->addAction(m_ui->action_Quit);
    menu->setLayoutDirection(Qt::LeftToRight);
    m_menuSetup->setMenu(menu);
    m_ui->toolMenuBar->addWidget(m_menuSetup);
}

void ActionManager::createScanSectionBar()
{
    QAction *action;

    action = new QAction(m_ui);
    action->setText(tr("Scan"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    m_collectionsScanSection.insert("scan-action", action);
    connect(action, SIGNAL(triggered(bool)), m_ui, SLOT(startScan()));
    action->setEnabled(false);
    m_ui->scanToolBar->addAction(action);

    // save menu
    m_saveTool = new QToolButton(m_ui);
    m_saveTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_saveTool->setPopupMode(QToolButton::InstantPopup);
    m_saveTool->setText(tr("Save"));
    m_saveTool->setIcon(QIcon(QString::fromUtf8(":/images/images/save_all.png")));

    QMenu *menuSave = new QMenu(m_ui);
    menuSave->addAction(m_ui->actionSave_Menu);
    menuSave->addAction(m_ui->actionSave_As_Menu);
    m_saveTool->setMenu(menuSave);

    m_ui->scanToolBar->addWidget(m_saveTool);
    m_ui->scanToolBar->addSeparator();

    // clear action
    action = new QAction(m_ui);
    action->setText(tr("Clear History"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/edit-clear-list.png")));
    m_collectionsScanSection.insert("clearHistory-action", action);
    connect(action, SIGNAL(triggered(bool)), m_ui, SLOT(clearAll()));
    action->setEnabled(false);
    m_ui->scanToolBar->addAction(action);

    // profiler menu
    m_profilerTool = new QToolButton(m_ui);
    m_profilerTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_profilerTool->setPopupMode(QToolButton::InstantPopup);
    m_profilerTool->setText(tr("Profile"));
    m_profilerTool->setIcon(QIcon(QString::fromUtf8(":/images/images/documentation.png")));

    QMenu *menuProfiler = new QMenu(m_ui);
    menuProfiler->addAction(m_ui->actionNew_Profile);
    menuProfiler->addAction(m_ui->actionEdit_Profile);
    m_profilerTool->setMenu(menuProfiler);

    m_ui->scanToolBar->addWidget(m_profilerTool);

    // bookmark menu
    m_bookmarksTool = new QToolButton(m_ui);
    m_bookmarksTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_bookmarksTool->setPopupMode(QToolButton::InstantPopup);
    m_bookmarksTool->setText(tr("Bookmark"));
    m_bookmarksTool->setIcon(QIcon(QString::fromUtf8(":/images/images/bookmark_add.png")));
    m_bookmarksTool->setMenu(m_ui->menu_Bookmaks);

    m_ui->toolBarBook->addWidget(m_bookmarksTool);
}

void ActionManager::createDiscoverBar()
{
    // discover toolbar
    m_discoverToolBar = new QToolBar(m_ui);
    m_discoverToolBar->setObjectName(QString::fromUtf8("discoverToolBar"));
    m_discoverToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_discoverToolBar->setLayoutDirection(Qt::LeftToRight);
    m_discoverToolBar->setStyleSheet(QString::fromUtf8("QToolBar { border: 0px; }"));
    m_discoverToolBar->setMovable(false);
    m_discoverToolBar->setIconSize(QSize(22, 22));
    m_discoverToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_ui->addToolBar(Qt::TopToolBarArea, m_discoverToolBar);
    m_discoverToolBar->setVisible(false);

    // scan discover action
    m_discoverScanTool = new QToolButton(m_ui);
    m_discoverScanTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_discoverScanTool->setPopupMode(QToolButton::InstantPopup);
    m_discoverScanTool->setText(tr("Scan Options"));
    m_discoverScanTool->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));

    QMenu *menuScanDiscover = new QMenu(m_ui);
    QAction *action;

    action = new QAction(m_ui);
    action->setText(tr("Scan selected IP/s"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    m_collectionsDiscover.insert("scan-single", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_discoverManager, SLOT(scanSingleDiscoveredIp()));
    action->setEnabled(false);
    menuScanDiscover->addAction(action);

    action = new QAction(m_ui);
    action->setText(tr("Scan all discovered IP/s"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    m_collectionsDiscover.insert("scan-all", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_discoverManager, SLOT(scanAllDiscoveredIps()));
    action->setEnabled(false);
    menuScanDiscover->addAction(action);

    m_discoverScanTool->setMenu(menuScanDiscover);
    m_discoverToolBar->addWidget(m_discoverScanTool);
    m_discoverToolBar->addSeparator();

    action = new QAction(m_ui);
    action->setText(tr("Save IP list"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/save_all.png")));
    m_collectionsDiscover.insert("save-ips", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_discoverManager, SLOT(saveXmlIpsList()));
    action->setEnabled(false);
    m_discoverToolBar->addAction(action);

    action = new QAction(m_ui);
    action->setText(tr("Load IP list"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/folder_open.png")));
    m_collectionsDiscover.insert("load-ips", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_discoverManager, SLOT(loadXmlIpsList()));
    action->setEnabled(true);
    m_discoverToolBar->addAction(action);
}

void ActionManager::disableBottomUiToggleActions()
{
    m_bottomUiToggleBar->setVisible(false);
}

void ActionManager::enableBottomUiToggleActions()
{
    m_bottomUiToggleBar->setVisible(true);
}
