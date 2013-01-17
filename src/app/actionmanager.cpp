/*
Copyright 2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "actionmanager.h"
#include "mainwindow.h"

ActionManager::ActionManager(MainWindow* parent)
    : QObject(parent), m_ui(parent)
{
    m_bottomUiToggleBar = new QToolBar(m_ui);
    m_bottomSectionBar = new QToolBar(m_ui);
    m_scanToolBar = new QToolBar(m_ui);
    m_bookmarkToolBar = new QToolBar(m_ui);
    m_globalMenuToolBar = new QToolBar(m_ui);
    m_vulnerabilityToolBar = new QToolBar(m_ui);
    m_discoverToolBar = new QToolBar(m_ui);

    m_bottomUiToggleBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_bottomUiToggleBar->setStyleSheet("QToolBar { border: 0px; }");
    m_bottomUiToggleBar->setFloatable(false);
    m_bottomUiToggleBar->setMovable(false);

    m_bottomSectionBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_bottomSectionBar->setStyleSheet("QToolBar { border: 0px; }");
    m_bottomSectionBar->setFloatable(false);
    m_bottomSectionBar->setMovable(false);

    m_scanToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_scanToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_scanToolBar->setStyleSheet("QToolBar { border: 0px; }");
    m_scanToolBar->setFloatable(false);
    m_scanToolBar->setMovable(false);
    m_scanToolBar->setIconSize(QSize(22, 22));

    m_bookmarkToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_bookmarkToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    m_bookmarkToolBar->setStyleSheet("QToolBar { border: 0px; }");
    m_bookmarkToolBar->setFloatable(false);
    m_bookmarkToolBar->setMovable(false);
    m_bookmarkToolBar->setIconSize(QSize(22, 22));

    m_globalMenuToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_globalMenuToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_globalMenuToolBar->setStyleSheet("QToolBar { border: 0px; }");
    m_globalMenuToolBar->setFloatable(false);
    m_globalMenuToolBar->setMovable(false);
    m_globalMenuToolBar->setIconSize(QSize(22, 22));

    m_vulnerabilityToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_vulnerabilityToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_vulnerabilityToolBar->setStyleSheet("QToolBar { border: 0px; }");
    m_vulnerabilityToolBar->setFloatable(false);
    m_vulnerabilityToolBar->setMovable(false);
    m_vulnerabilityToolBar->setIconSize(QSize(22, 22));

    // discover toolbar
    m_discoverToolBar->setObjectName(QString::fromUtf8("discoverToolBar"));
    m_discoverToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_discoverToolBar->setLayoutDirection(Qt::LeftToRight);
    m_discoverToolBar->setStyleSheet(QString::fromUtf8("QToolBar { border: 0px; }"));
    m_discoverToolBar->setMovable(false);
    m_discoverToolBar->setIconSize(QSize(22, 22));
    m_discoverToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_discoverToolBar->setVisible(false);

    //m_ui->addToolBar(Qt::BottomToolBarArea, m_bottomSectionBar);
    m_ui->addToolBar(Qt::LeftToolBarArea, m_bottomSectionBar);
    m_ui->addToolBar(Qt::BottomToolBarArea, m_bottomUiToggleBar);
    m_ui->addToolBar(Qt::TopToolBarArea, m_scanToolBar);
    m_ui->addToolBar(Qt::TopToolBarArea, m_vulnerabilityToolBar);
    m_ui->addToolBar(Qt::TopToolBarArea, m_bookmarkToolBar);
    m_ui->addToolBar(Qt::TopToolBarArea, m_discoverToolBar);
    m_ui->addToolBar(Qt::TopToolBarArea, m_globalMenuToolBar);

    connect(m_ui->m_mouseFilter, SIGNAL(mouseRightClickEvent(QObject*)),
            this, SLOT(mouseClickEvent(QObject*)));

    setupActions();
    setupMenuBar();
}

void ActionManager::mouseClickEvent(QObject* obj)
{
    if (obj == m_ui->m_bookmark->m_scanBookmarkWidget->treeLogH) {
        if (m_ui->m_bookmark->m_scanBookmarkWidget->treeLogH->currentItem() != NULL) {
            scanBookmarkContextMenu();
        }
    }

    if (obj == m_ui->m_bookmark->m_scanBookmarkWidget->treeBookPar) {
        if (m_ui->m_bookmark->m_scanBookmarkWidget->treeBookPar->currentItem() != NULL) {
            parametersBookmarkContextMenu();
        }
    }

    if (obj == m_ui->m_bookmark->m_vulnBookmarkWidget->treeBookVuln) {
        if (m_ui->m_bookmark->m_vulnBookmarkWidget->treeBookVuln->currentItem() != NULL) {
            servicesContextMenu();
        }
    }

    if (obj == m_ui->m_bookmark->m_vulnBookmarkWidget->treeWidgetVulnUrl) {
        if (m_ui->m_bookmark->m_vulnBookmarkWidget->treeWidgetVulnUrl->currentItem() != NULL) {
            vulnerabilityUrlContextMenu();
        }
    }

    if (obj == m_ui->m_scanWidget->treeHostDet) {
        if (m_ui->m_scanWidget->treeHostDet->currentItem() != NULL) {
            scanHostInfoContextMenu();
        }
    }

    if (obj == m_ui->m_scanWidget->listWscan) {
        if (m_ui->m_scanWidget->listWscan->currentItem() != NULL) {
            scanPortsInfoContextMenu();
        }
    }

    if (obj == m_ui->m_scanWidget->listScan) {
        if (m_ui->m_scanWidget->listScan->currentItem() != NULL) {
            scanFullOutputContextMenu();
        }
    }
}

void ActionManager::setupActions()
{
    QAction *action;

    action = new QAction(m_ui);
    action->setText(tr("Scan"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    m_collectionsScanSection.insert("scan-action", action);
    connect(action, SIGNAL(triggered(bool)), m_ui, SLOT(startScan()));
    action->setEnabled(false);

    // clear action
    action = new QAction(m_ui);
    action->setText(tr("Clear History"));
    action->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/images/images/edit-clear-list.png")));
    m_collectionsScanSection.insert("clearHistory-action", action);
    connect(action, SIGNAL(triggered(bool)), m_ui, SLOT(clearAll()));
    action->setEnabled(false);

    action = new QAction(m_ui);
    action->setText(tr("S&ave Scan"));
    action->setIcon(QIcon::fromTheme("document-save", QIcon(":/images/images/save_all.png")));
    m_collectionsScanSection.insert("save-action", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_parser, SLOT(callSaveSingleLogWriter()));
    action->setEnabled(false);

    action = new QAction(m_ui);
    action->setText(tr("Save all scans to &Directory"));
    action->setIcon(QIcon::fromTheme("document-save-as", QIcon(":/images/images/document-save-as.png")));
    m_collectionsScanSection.insert("saveAll-action", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_parser, SLOT(callSaveAllLogWriter()));
    action->setEnabled(false);

    m_menuBookmark = new QMenu(m_ui);
    action = new QAction(m_ui);
    action->setText(tr("&Add host to bookmark"));
    action->setIcon(QIcon::fromTheme("address-book-new", QIcon(":/images/images/bookmark_add.png")));
    m_collectionsScanSection.insert("bookmarkAddHost-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_bookmark, SLOT(saveHostnameItemToBookmark()));

    action = new QAction(m_ui);
    action->setText(tr("Add service to &bookmark"));
    action->setIcon(QIcon::fromTheme("address-book-new", QIcon(":/images/images/bookmark_add.png")));
    m_collectionsScanSection.insert("bookmarkAddService-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_bookmark, SLOT(saveServiceItemToBookmark()));

    action = new QAction(m_ui);
    action->setText(tr("Add &parameters to bookmark"));
    action->setIcon(QIcon::fromTheme("address-book-new", QIcon(":/images/images/bookmark_add.png")));
    m_collectionsScanSection.insert("bookmarkAddParameters-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_bookmark, SLOT(startParametersToBookmarksDialog()));

    action = new QAction(m_ui);
    action->setText(tr("Add vulnerability search url"));
    action->setIcon(QIcon::fromTheme("address-book-new", QIcon(":/images/images/bookmark_add.png")));
    m_collectionsScanSection.insert("bookmarkAddVulnUrl-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(showAddUrlUi()));

    // Profiler QActions
    action = new QAction(m_ui);
    action->setText(tr("New Profile"));
    action->setIcon(QIcon::fromTheme("document-new", QIcon(":/images/images/document-new.png")));
    m_collectionsScanSection.insert("newProfile-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(newProfile()));

    action = new QAction(m_ui);
    action->setText(tr("Edit Profile"));
    action->setIcon(QIcon::fromTheme("document-properties", QIcon(":/images/images/document-properties.png")));
    m_collectionsScanSection.insert("editProfile-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(editProfile()));

    // discover Actions
    action = new QAction(m_ui);
    action->setText(tr("Scan selected IP/s"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    m_collectionsDiscover.insert("scan-single", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_discoverManager, SLOT(scanSingleDiscoveredIp()));
    action->setEnabled(false);

    action = new QAction(m_ui);
    action->setText(tr("Scan all discovered IP/s"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    m_collectionsDiscover.insert("scan-all", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_discoverManager, SLOT(scanAllDiscoveredIps()));
    action->setEnabled(false);

    action = new QAction(m_ui);
    action->setText(tr("Save IP list"));
    action->setIcon(QIcon::fromTheme("document-save", QIcon(":/images/images/save_all.png")));
    m_collectionsDiscover.insert("save-ips", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_discoverManager, SLOT(saveXmlIpsList()));
    action->setEnabled(false);

    action = new QAction(m_ui);
    action->setText(tr("Load IP list"));
    action->setIcon(QIcon::fromTheme("folder-open", QIcon(":/images/images/folder_open.png")));
    m_collectionsDiscover.insert("load-ips", action);
    connect(action, SIGNAL(triggered(bool)), m_ui->m_discoverManager, SLOT(loadXmlIpsList()));
    action->setEnabled(true);

    // Vulnerability Actions
    action = new QAction(m_ui);
    action->setIcon(QIcon::fromTheme("system-search", QIcon(":/images/images/viewmag.png")));
    action->setIconText(tr("Search"));
    action->setEnabled(false);
    m_collectionsVulnerability.insert("search-act", action);
    connect(action, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(searchVulnerabilityFromCombo()));

    action = new QAction(m_ui);
    action->setIcon(QIcon::fromTheme("go-previous", QIcon(":/images/images/go-previous.png")));
    action->setIconText(tr("Back"));
    action->setEnabled(false);
    m_collectionsVulnerability.insert("back-act", action);
    connect(action, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(tabWebBack()));

    action = new QAction(m_ui);
    action->setIcon(QIcon::fromTheme("go-next", QIcon(":/images/images/go-next.png")));
    action->setIconText(tr("Forward"));
    action->setEnabled(false);
    m_collectionsVulnerability.insert("forward-act", action);
    connect(action, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(tabWebForward()));

    action = new QAction(m_ui);
    action->setIcon(QIcon::fromTheme("process-stop", QIcon(":/images/images/button_cancel.png")));
    action->setIconText(tr("Stop"));
    action->setEnabled(false);
    m_collectionsVulnerability.insert("stop-act", action);
    connect(action, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(tabWebStop()));

    // global QActions
    action = new QAction(m_ui);
    action->setText(tr("&Quit"));
    action->setIcon(QIcon::fromTheme("application-exit", QIcon(":/images/images/window-close.png")));
    action->setShortcut(Qt::CTRL + Qt::Key_Q);
    m_collectionsScanSection.insert("quit-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(close()));

    action = new QAction(m_ui);
    action->setText(tr("&Preferences"));
    action->setIcon(QIcon::fromTheme("preferences-other", QIcon(":/images/images/tool.png")));
    m_collectionsScanSection.insert("preferences-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(startPreferencesDialog()));

    action = new QAction(m_ui);
    action->setText(tr("F&ull Screen Mode"));
    action->setCheckable(true);
    action->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_F11);
    action->setIcon(QIcon::fromTheme("view-fullscreen", QIcon(":/images/images/view-fullscreen.png")));
    m_collectionsScanSection.insert("fullscreen-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(setFullScreen()));

    action = new QAction(m_ui);
    action->setText(tr("Show &Menu Bar"));
    action->setCheckable(true);
    action->setShortcut(Qt::CTRL + Qt::Key_M);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/show-menu.png")));
    m_collectionsScanSection.insert("showmenubar-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(updateMenuBar()));

    action = new QAction(m_ui);
    action->setText(tr("Section Scan"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    m_collectionsScanSection.insert("sectionScan-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(updateScanSection()));

    action = new QAction(m_ui);
    action->setText(tr("Section Vulnerabilities"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    m_collectionsScanSection.insert("sectionVuln-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(updateVulnerabilitySection()));

    action = new QAction(m_ui);
    action->setText(tr("Section Discover"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
    m_collectionsScanSection.insert("sectionDiscover-action", action);
    connect(action, SIGNAL(triggered()), m_ui, SLOT(updateDiscoverSection()));

    // Help menu actions
    action = new QAction(m_ui);
    action->setText(tr("Report a bug"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/tools-report-bug.png")));
    m_collectionsScanSection.insert("bug-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_utilities, SLOT(showBugUrl()));

    action = new QAction(m_ui);
    action->setText(tr("Visit Website"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
    m_collectionsScanSection.insert("home-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_utilities, SLOT(showHomepageUrl()));

    action = new QAction(m_ui);
    action->setText(tr("Documentation"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/book2.png")));
    m_collectionsScanSection.insert("documentation-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_utilities, SLOT(showDocumentationUrl()));

    action = new QAction(m_ui);
    action->setText(tr("Donate Money"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/kwalletmanager.png")));
    m_collectionsScanSection.insert("donate-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_utilities, SLOT(showDonateUrl()));

    action = new QAction(m_ui);
    action->setText(tr("&About nmapsi4"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/icons/64x64/nmapsi4.png")));
    m_collectionsScanSection.insert("aboutui-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_utilities, SLOT(about()));

    action = new QAction(m_ui);
    action->setText(tr("About &Qt"));
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
    m_collectionsScanSection.insert("aboutqt-action", action);
    connect(action, SIGNAL(triggered()), m_ui->m_utilities, SLOT(aboutQt()));
}

void ActionManager::setupMenuBar()
{
    QMenuBar *menubar = new QMenuBar(m_ui);
    // File menu
    m_menuFile = new QMenu(menubar);
    m_menuFile->setTitle(tr("&File"));
    m_menuFile->addAction(m_collectionsScanSection.value("quit-action"));

    // Settings menu
    m_menuSettings = new QMenu(menubar);
    m_menuSettings->setTitle(tr("S&ettings"));
    m_menuSettings->addAction(m_collectionsScanSection.value("fullscreen-action"));
    m_menuSettings->addAction(m_collectionsScanSection.value("showmenubar-action"));
    m_menuSettings->addSeparator();
    m_menuSettings->addAction(m_collectionsScanSection.value("preferences-action"));

    // Tools menu
    m_menuTools = new QMenu(menubar);
    m_menuTools->setTitle(tr("&Tools"));
    m_menuTools->addAction(m_collectionsScanSection.value("sectionScan-action"));
    m_menuTools->addAction(m_collectionsScanSection.value("sectionVuln-action"));
    m_menuTools->addAction(m_collectionsScanSection.value("sectionDiscover-action"));

    // Help menu
    m_menuHelp = new QMenu(menubar);
    m_menuHelp->setTitle(tr("&Help"));
    m_menuHelp->addAction(m_collectionsScanSection.value("bug-action"));
    m_menuHelp->addAction(m_collectionsScanSection.value("home-action"));
    m_menuHelp->addAction(m_collectionsScanSection.value("documentation-action"));
    m_menuHelp->addAction(m_collectionsScanSection.value("donate-action"));
    m_menuHelp->addSeparator();
    m_menuHelp->addAction(m_collectionsScanSection.value("aboutui-action"));
    m_menuHelp->addAction(m_collectionsScanSection.value("aboutqt-action"));

    menubar->addAction(m_menuFile->menuAction());
    menubar->addAction(m_menuTools->menuAction());
    menubar->addAction(m_menuSettings->menuAction());
    menubar->addAction(m_menuHelp->menuAction());

    m_ui->setMenuBar(menubar);
}

void ActionManager::createSectionsBar()
{
    QToolButton* actionButt;

    m_bottomSectionBar->setIconSize(QSize(26,26));
    // FIXME: qtoolbutton problem with textunderIcon

    actionButt = new QToolButton(m_bottomSectionBar);
    actionButt->setIcon(QIcon(QString::fromUtf8(":/images/icons/128x128/nmapsi4.png")));
    actionButt->setText(tr("Welcome"));
    //actionButt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionButt->setToolButtonStyle(Qt::ToolButtonIconOnly);
    actionButt->setCheckable(true);
    m_collectionsButton.insert("welcome-sez", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(updateWelcomeSection()));
    m_bottomSectionBar->addWidget(actionButt);

    actionButt = new QToolButton(m_bottomSectionBar);
    actionButt->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    actionButt->setText(tr("Scan host(s)"));
    actionButt->setToolButtonStyle(Qt::ToolButtonIconOnly);
    //actionButt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionButt->setToolTip(tr("Scan host(s)"));
    actionButt->setCheckable(true);
    m_collectionsButton.insert("scan-sez", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(updateScanSection()));
    m_bottomSectionBar->addWidget(actionButt);

    actionButt = new QToolButton(m_bottomSectionBar);
    actionButt->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    actionButt->setText(tr("Vulnerability"));
    actionButt->setToolButtonStyle(Qt::ToolButtonIconOnly);
    //actionButt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionButt->setToolTip(tr("Check Vulnerabilities"));
    actionButt->setCheckable(true);
    m_collectionsButton.insert("vuln-sez", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(updateVulnerabilitySection()));
    m_bottomSectionBar->addWidget(actionButt);

    actionButt = new QToolButton(m_bottomSectionBar);
    actionButt->setIcon(QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
    actionButt->setText(tr("Discover"));
    actionButt->setToolButtonStyle(Qt::ToolButtonIconOnly);
    //actionButt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionButt->setToolTip(tr("Network Discover"));
    actionButt->setCheckable(true);
    m_collectionsButton.insert("discover-sez", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(updateDiscoverSection()));
    m_bottomSectionBar->addWidget(actionButt);

    // toggle action
    actionButt = new QToolButton(m_ui);
    actionButt->setText(tr("Show/Hide host list"));
    actionButt->setIcon(QIcon(QString::fromUtf8(":/images/images/view-restore.png")));
    actionButt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionButt->setToolTip(tr("Show/Hide Host List"));
    actionButt->setCheckable(true);
    m_collectionsButton.insert("scan-list", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(resizeScanListWidgetEvent()));
    m_bottomUiToggleBar->addWidget(actionButt);

    actionButt = new QToolButton(m_ui);
    actionButt->setText(tr("Show/Hide host details"));
    actionButt->setIcon(QIcon(QString::fromUtf8(":/images/images/view-restore.png")));
    actionButt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionButt->setToolTip(tr("Show/Hide Host Details"));
    actionButt->setCheckable(true);
    m_collectionsButton.insert("details-list", actionButt);
    connect(actionButt, SIGNAL(clicked(bool)), m_ui, SLOT(resizeHostDetailsWidgetEvent()));
    m_bottomUiToggleBar->addWidget(actionButt);
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
    menu->addAction(m_collectionsScanSection.value("sectionScan-action"));
    menu->addAction(m_collectionsScanSection.value("sectionVuln-action"));
    menu->addAction(m_collectionsScanSection.value("sectionDiscover-action"));
    menu->addSeparator();
    menu->addAction(m_collectionsScanSection.value("preferences-action"));
    menu->addSeparator();
    menu->addMenu(m_menuHelp);
    menu->addSeparator();
    menu->addAction(m_collectionsScanSection.value("showmenubar-action"));
    menu->addSeparator();
    menu->addAction(m_collectionsScanSection.value("quit-action"));
    menu->setLayoutDirection(Qt::LeftToRight);
    m_menuSetup->setMenu(menu);
    m_globalMenuToolBar->addWidget(m_menuSetup);
}

void ActionManager::createScanSectionBar()
{
    m_scanToolBar->addAction(m_collectionsScanSection.value("scan-action"));

    // save menu
    m_saveTool = new QToolButton(m_ui);
    m_saveTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_saveTool->setPopupMode(QToolButton::InstantPopup);
    m_saveTool->setText(tr("Save"));
    m_saveTool->setIcon(QIcon(QString::fromUtf8(":/images/images/save_all.png")));

    QMenu *menuSave = new QMenu(m_ui);
    menuSave->addAction(m_collectionsScanSection.value("save-action"));
    menuSave->addAction(m_collectionsScanSection.value("saveAll-action"));
    m_saveTool->setMenu(menuSave);

    m_scanToolBar->addWidget(m_saveTool);
    m_scanToolBar->addSeparator();

    // clear action
    m_scanToolBar->addAction(m_collectionsScanSection.value("clearHistory-action"));

    // profiler menu
    m_profilerTool = new QToolButton(m_ui);
    m_profilerTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_profilerTool->setPopupMode(QToolButton::InstantPopup);
    m_profilerTool->setText(tr("Profile"));
    m_profilerTool->setIcon(QIcon(QString::fromUtf8(":/images/images/documentation.png")));

    QMenu *menuProfiler = new QMenu(m_ui);
    menuProfiler->addAction(m_collectionsScanSection.value("newProfile-action"));
    menuProfiler->addAction(m_collectionsScanSection.value("editProfile-action"));
    m_profilerTool->setMenu(menuProfiler);

    m_scanToolBar->addWidget(m_profilerTool);

    // bookmark menu
    m_bookmarksTool = new QToolButton(m_ui);
    m_bookmarksTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_bookmarksTool->setPopupMode(QToolButton::InstantPopup);
    m_bookmarksTool->setText(tr("Bookmark"));
    m_bookmarksTool->setIcon(QIcon::fromTheme("user-bookmarks", QIcon(":/images/images/bookmark_add.png")));

    m_menuBookmark = new QMenu(m_ui);
    m_menuBookmark->addAction(m_collectionsScanSection.value("bookmarkAddHost-action"));
    m_menuBookmark->addAction(m_collectionsScanSection.value("bookmarkAddService-action"));
    m_menuBookmark->addAction(m_collectionsScanSection.value("bookmarkAddParameters-action"));
    m_menuBookmark->addAction(m_collectionsScanSection.value("bookmarkAddVulnUrl-action"));

    m_bookmarksTool->setMenu(m_menuBookmark);
    m_bookmarkToolBar->addWidget(m_bookmarksTool);
}

void ActionManager::createDiscoverBar()
{
    // scan discover action
    m_discoverScanTool = new QToolButton(m_ui);
    m_discoverScanTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_discoverScanTool->setPopupMode(QToolButton::InstantPopup);
    m_discoverScanTool->setText(tr("Scan Options"));
    m_discoverScanTool->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));

    QMenu *menuScanDiscover = new QMenu(m_ui);

    menuScanDiscover->addAction(m_collectionsDiscover.value("scan-single"));
    menuScanDiscover->addAction(m_collectionsDiscover.value("scan-all"));

    m_discoverScanTool->setMenu(menuScanDiscover);
    m_discoverToolBar->addWidget(m_discoverScanTool);
    m_discoverToolBar->addSeparator();

    m_discoverToolBar->addAction(m_collectionsDiscover.value("save-ips"));
    m_discoverToolBar->addAction(m_collectionsDiscover.value("load-ips"));
}

void ActionManager::createVulnerabilityBar()
{
    m_vulnerabilityToolBar->addAction(m_collectionsVulnerability.value("search-act"));
    m_vulnerabilityToolBar->addAction(m_collectionsVulnerability.value("back-act"));
    m_vulnerabilityToolBar->addAction(m_collectionsVulnerability.value("forward-act"));
    m_vulnerabilityToolBar->addAction(m_collectionsVulnerability.value("stop-act"));
}

void ActionManager::disableBottomUiToggleActions()
{
    m_bottomUiToggleBar->setVisible(false);
}

void ActionManager::enableBottomUiToggleActions()
{
    m_bottomUiToggleBar->setVisible(true);
}

void ActionManager::disableScanSectionToolBar()
{
    m_scanToolBar->setVisible(false);
}

void ActionManager::enableScanSectionToolBar()
{
    m_scanToolBar->setVisible(true);
}

void ActionManager::disableBookmarkToolBar()
{
    m_bookmarkToolBar->setVisible(false);
}

void ActionManager::enableBookmarkToolBar()
{
    m_bookmarkToolBar->setVisible(true);
}

void ActionManager::disableGlobalMenuToolBar()
{
    m_globalMenuToolBar->setVisible(false);
}

void ActionManager::enableGlobalMenuToolBar()
{
    if (!m_ui->menuBar()->isVisible()) {
        m_globalMenuToolBar->setVisible(true);
    }
}

void ActionManager::disableVulnerabilityToolBar()
{
    m_vulnerabilityToolBar->setVisible(false);
}

void ActionManager::enableVulnerabilityToolBar()
{
    m_vulnerabilityToolBar->setVisible(true);
}

void ActionManager::disableBookmarkMenu()
{
    m_menuBookmark->setEnabled(false);
}

void ActionManager::enableScanBookmarkMenu()
{
    m_menuBookmark->setEnabled(true);
    m_collectionsScanSection.value("bookmarkAddHost-action")->setVisible(true);
    m_collectionsScanSection.value("bookmarkAddService-action")->setVisible(false);
    m_collectionsScanSection.value("bookmarkAddParameters-action")->setVisible(true);
    m_collectionsScanSection.value("bookmarkAddVulnUrl-action")->setVisible(false);
}

void ActionManager::enableVulnerabilityBookmarkMenu()
{
    m_menuBookmark->setEnabled(true);
    m_collectionsScanSection.value("bookmarkAddHost-action")->setVisible(false);
    m_collectionsScanSection.value("bookmarkAddService-action")->setVisible(true);
    m_collectionsScanSection.value("bookmarkAddParameters-action")->setVisible(false);
    m_collectionsScanSection.value("bookmarkAddVulnUrl-action")->setVisible(true);
}

void ActionManager::disableSaveActions()
{
    m_collectionsScanSection.value("save-action")->setEnabled(false);
    m_collectionsScanSection.value("saveAll-action")->setEnabled(false);
}

void ActionManager::enableSaveActions()
{
    m_collectionsScanSection.value("save-action")->setEnabled(true);
    m_collectionsScanSection.value("saveAll-action")->setEnabled(true);
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

void ActionManager::scanHostInfoContextMenu()
{
    QAction copyHostDetail(m_ui);
    copyHostDetail.setIcon(QIcon::fromTheme("edit-copy", QIcon(":/images/images/edit-copy.png")));
    copyHostDetail.setIconText(tr("Copy"));

    connect(&copyHostDetail, SIGNAL(triggered()), m_ui, SLOT(copyTextFromHostInfoTree()));

    QMenu menuHostInfo(m_ui);
    menuHostInfo.addAction(&copyHostDetail);
    menuHostInfo.exec(QCursor::pos());
}

void ActionManager::scanPortsInfoContextMenu()
{
    QAction copyHostDetail(m_ui);
    copyHostDetail.setIcon(QIcon::fromTheme("edit-copy", QIcon(":/images/images/edit-copy.png")));
    copyHostDetail.setIconText(tr("Copy"));

    QAction checkVuln(m_ui);
    checkVuln.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    checkVuln.setIconText(tr("Check Vulnerabilities"));

    QAction openUrl(m_ui);
    openUrl.setIcon(QIcon(QString::fromUtf8(":/images/images/network-workgroup.png")));
    openUrl.setIconText(tr("Open address via http"));

    connect(&checkVuln, SIGNAL(triggered()), m_ui->m_vulnerability, SLOT(checkVulnerabilitiesFromPortsTree()));
    connect(&copyHostDetail, SIGNAL(triggered()), m_ui, SLOT(copyTextFromScanPortsTree()));
    connect(&openUrl, SIGNAL(triggered()), m_ui, SLOT(openUrlFromScanPortsTree()));

    openUrl.setEnabled(false);

    // check for http port
    foreach (QTreeWidgetItem* item, m_ui->m_scanWidget->listWscan->selectedItems()) {
        if (item->text(2).contains("http") && !item->text(2).contains("ssl")) {
            openUrl.setEnabled(true);
            break;
        }
    }

    QMenu menuHostInfo(m_ui);
    menuHostInfo.addAction(&checkVuln);
    menuHostInfo.addSeparator();
    menuHostInfo.addAction(&copyHostDetail);
    menuHostInfo.addAction(&openUrl);
    menuHostInfo.exec(QCursor::pos());
}

void ActionManager::scanFullOutputContextMenu()
{
    QAction copyHostDetail(m_ui);
    copyHostDetail.setIcon(QIcon::fromTheme("edit-copy", QIcon(":/images/images/edit-copy.png")));
    copyHostDetail.setIconText(tr("Copy"));

    connect(&copyHostDetail, SIGNAL(triggered()), m_ui, SLOT(copyTextFromScanFullOutputTree()));

    QMenu menuHostInfo(m_ui);
    menuHostInfo.addAction(&copyHostDetail);
    menuHostInfo.exec(QCursor::pos());

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
