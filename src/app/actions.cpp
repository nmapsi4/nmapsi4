/***************************************************************************
 *   Copyright (C) 2010-2011 by Francesco Cecconi                          *
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


void MainWindow::menuScanBook()
{
    QAction removeBook(this);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Host"));

    QAction scanBook(this);
    scanBook.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    scanBook.setIconText(tr("Scan Host"));

    connect(&scanBook, SIGNAL(triggered()), this, SLOT(takeHostFromBookmark()));
    connect(&removeBook, SIGNAL(triggered()), m_bookmark, SLOT(deleteItemFromBookmark()));

    QMenu menuBook(this);
    menuBook.addAction(&scanBook);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}

void MainWindow::menuVulnBook()
{
    QAction removeBook(this);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Service"));

    QAction addBook(this);
    addBook.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    addBook.setIconText(tr("Search for vulnerabilities"));

    connect(&addBook, SIGNAL(triggered()), m_vulnerability, SLOT(callVulnCheck()));
    connect(&removeBook, SIGNAL(triggered()), m_bookmark, SLOT(deleteItemFromBookmark()));

    QMenu menuBook(this);
    menuBook.addAction(&addBook);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}

void MainWindow::menuParBook()
{
    QAction removeBook(this);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Scan Parameters"));

    connect(&removeBook, SIGNAL(triggered()), m_bookmark, SLOT(deleteParametersFromBookmark()));

    QMenu menuBook(this);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}

void MainWindow::menuServiceMain()
{
    QAction checkVuln(this);
    checkVuln.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    checkVuln.setIconText(tr("Check Vulnerability"));

    connect(&checkVuln, SIGNAL(triggered()), m_vulnerability, SLOT(objVulnButton()));

    QMenu menuVulnMain(this);
    menuVulnMain.addAction(&checkVuln);
    menuVulnMain.exec(QCursor::pos());
}

void MainWindow::menuVulnUrlBookmark()
{
    QAction removeVulnUrl(this);
    removeVulnUrl.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeVulnUrl.setIconText(tr("Remove url"));
    connect(&removeVulnUrl, SIGNAL(triggered()), m_vulnerability, SLOT(removeUrlToBookmarks()));

    QMenu menuVulnUrl(this);
    menuVulnUrl.addAction(&removeVulnUrl);
    menuVulnUrl.exec(QCursor::pos());
}

void MainWindow::createBar()
{
    QPushButtonOrientated* actionButt;
    const int verticalButtonWidth = 25;

    actionButt = new QPushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/network_local.png")),tr("Scan host(s)"));
    actionButt->setToolTip(tr("Scan host(s)"));
    actionButt->setOrientation(Qt::Vertical);
    actionButt->setFlat(true);
    actionButt->setMaximumWidth(verticalButtonWidth);
    actionButt->setCheckable(true);
    actionButt->setStyleSheet(verticalStyleSheet);
    m_collectionsButton.insert("scan-sez",actionButt);
    connect(actionButt, SIGNAL(clicked(bool)),this, SLOT(updateSezScan()));
    sezBar->addWidget(actionButt);

    actionButt = new QPushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")),tr("Network Discover"));
    actionButt->setToolTip(tr("Network Discover"));
    actionButt->setOrientation(Qt::Vertical);
    actionButt->setFlat(true);
    actionButt->setMaximumWidth(verticalButtonWidth);
    actionButt->setCheckable(true);
    actionButt->setStyleSheet(verticalStyleSheet);
    m_collectionsButton.insert("discover-sez",actionButt);
    connect(actionButt, SIGNAL(clicked(bool)),this, SLOT(updateSezDiscover()));
    sezBar->addWidget(actionButt);

    actionButt = new QPushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")),tr("Vulnerability"));
    actionButt->setToolTip(tr("Check Vulnerabilities"));
    actionButt->setOrientation(Qt::Vertical);
    actionButt->setFlat(true);
    actionButt->setMaximumWidth(verticalButtonWidth);
    actionButt->setCheckable(true);
    actionButt->setStyleSheet(verticalStyleSheet);
    m_collectionsButton.insert("vuln-sez",actionButt);
    connect(actionButt, SIGNAL(clicked(bool)),this, SLOT(updateSezVuln()));
    sezBar->addWidget(actionButt);

    sezBar->setContextMenuPolicy(Qt::PreventContextMenu);

    actionButt = new QPushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/view-restore.png")),tr("Host List"));
    actionButt->setToolTip(tr("Show/Hide Host List"));
    actionButt->setOrientation(Qt::Vertical);
    actionButt->setMirrored(true);
    actionButt->setFlat(true);
    actionButt->setMaximumWidth(verticalButtonWidth);
    actionButt->setCheckable(true);
    actionButt->setStyleSheet(verticalStyleSheet);
    m_collectionsButton.insert("scan-list",actionButt);
    connect(actionButt, SIGNAL(clicked(bool)),this, SLOT(resizeScanListWidgetEvent()));
    toolBarTab->addWidget(actionButt);

    actionButt = new QPushButtonOrientated(QIcon(QString::fromUtf8(":/images/images/view-restore.png")),tr("Host Details"));
    actionButt->setToolTip(tr("Show/Hide Host Details"));
    actionButt->setOrientation(Qt::Vertical);
    actionButt->setMirrored(true);
    actionButt->setFlat(true);
    actionButt->setMaximumWidth(verticalButtonWidth);
    actionButt->setCheckable(true);
    actionButt->setStyleSheet(verticalStyleSheet);
    m_collectionsButton.insert("details-list",actionButt);
    connect(actionButt, SIGNAL(clicked(bool)),this, SLOT(resizeHostDetailsWidgetEvent()));
    toolBarTab->addWidget(actionButt);

    toolBarTab->setContextMenuPolicy(Qt::PreventContextMenu);
}

void MainWindow::createToolButtonSetup()
{
    // new QToolButton menu
    m_menuSetup = new QToolButton();
    m_menuSetup->setPopupMode(QToolButton::InstantPopup);
    m_menuSetup->setText(tr("Setup"));
    m_menuSetup->setIcon(QIcon(QString::fromUtf8(":/images/images/tool.png")));

    QMenu *menu = new QMenu();
    menu->addAction(action_Scan_2);
    menu->addAction(actionSave);
    menu->addAction(actionSave_As_Menu);
    menu->addSeparator();
    menu->addMenu(menu_Bookmaks);
    menu->addSeparator();
    menu->addAction(actionScan_section);
    menu->addAction(actionVulnerabilities_section);
    menu->addAction(actionSection_Discover);
    menu->addSeparator();
    menu->addAction(actionProfile);
    menu->addAction(actionNew_Profile);
    menu->addAction(actionEdit_Profile);
    menu->addSeparator();
    menu->addMenu(menu_Help);
    menu->addSeparator();
    menu->addAction(actionMenuBar);
    menu->addSeparator();
    menu->addAction(action_Quit);
    menu->setLayoutDirection(Qt::LeftToRight);
    m_menuSetup->setMenu(menu);

    toolMenuBar->addWidget(m_menuSetup);

    // profiler menu
    m_profilerTool = new QToolButton();
    m_profilerTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_profilerTool->setPopupMode(QToolButton::InstantPopup);
    m_profilerTool->setText(tr("Profile"));
    m_profilerTool->setIcon(QIcon(QString::fromUtf8(":/images/images/documentation.png")));

    QMenu *menuProfiler = new QMenu();
    menuProfiler->addAction(actionNew_Profile);
    menuProfiler->addAction(actionEdit_Profile);
    m_profilerTool->setMenu(menuProfiler);

    toolBar_2->addWidget(m_profilerTool);

    // save menu
    m_saveTool = new QToolButton();
    m_saveTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_saveTool->setPopupMode(QToolButton::InstantPopup);
    m_saveTool->setText(tr("Save"));
    m_saveTool->setIcon(QIcon(QString::fromUtf8(":/images/images/save_all.png")));

    QMenu *menuSave = new QMenu();
    menuSave->addAction(actionSave_Menu);
    menuSave->addAction(actionSave_As_Menu);
    m_saveTool->setMenu(menuSave);

    toolBar->addWidget(m_saveTool);
    toolBar->addSeparator();

    // bookmark menu
    m_bookmarksTool = new QToolButton();
    m_bookmarksTool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_bookmarksTool->setPopupMode(QToolButton::InstantPopup);
    m_bookmarksTool->setText(tr("Bookmark"));
    m_bookmarksTool->setIcon(QIcon(QString::fromUtf8(":/images/images/bookmark_add.png")));
    m_bookmarksTool->setMenu(menu_Bookmaks);

    toolBarBook->addWidget(m_bookmarksTool);
}
