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

#include "../mainwin.h"


void nmapClass::menuScanBook() 
{
    QAction removeBook(this);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Host"));

    QAction scanBook(this);
    scanBook.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    scanBook.setIconText(tr("Scan Host"));

    connect(&scanBook, SIGNAL(triggered()),
                this, SLOT(callScanH()));
    connect(&removeBook, SIGNAL(triggered()),
            this, SLOT(deleteBookMark()));

    QMenu menuBook(this);
    menuBook.addAction(&scanBook);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}

void nmapClass::menuVulnBook() 
{
    QAction removeBook(this);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Service"));

    QAction addBook(this);
    addBook.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    addBook.setIconText(tr("Search for vulnerabilities"));

    connect(&addBook, SIGNAL(triggered()),
                this, SLOT(callVulnCheck()));
    connect(&removeBook, SIGNAL(triggered()),
            this, SLOT(deleteBookMark()));

    QMenu menuBook(this);
    menuBook.addAction(&addBook);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}

void nmapClass::menuParBook() 
{
    QAction removeBook(this);
    removeBook.setIcon(QIcon(QString::fromUtf8(":/images/images/window-close.png")));
    removeBook.setIconText(tr("Remove Scan Paramenters"));

    QAction addBook(this);
    addBook.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    addBook.setIconText(tr("Use Parameters"));

    connect(&addBook, SIGNAL(triggered()),
                this, SLOT(callParFromBook()));
    connect(&removeBook, SIGNAL(triggered()),
            this, SLOT(deleteBookMarkPar()));

    QMenu menuBook(this);
    menuBook.addAction(&addBook);
    menuBook.addAction(&removeBook);

    menuBook.exec(QCursor::pos());
}

void nmapClass::menuServiceMain() 
{
    QAction checkVuln(this);
    checkVuln.setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    checkVuln.setIconText(tr("Check Vulnerability"));

    connect(&checkVuln, SIGNAL(triggered()),
                this, SLOT(objVulnButton()));

    QMenu menuVulnMain(this);
    menuVulnMain.addAction(&checkVuln);
    menuVulnMain.exec(QCursor::pos());
}

void nmapClass::createBar() 
{
    QAction* action;

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    action->setIconText(tr("Scan"));
    action->setToolTip(tr("Scan host(s)"));
    action->setCheckable(true);
    _collections.insert("scan-sez",action);
    connect(action, SIGNAL(triggered()),this, SLOT(updateSezScan()));
    sezBar->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/book.png")));
    action->setIconText(tr("Log"));
    action->setToolTip(tr("Scan Log"));
    action->setCheckable(true);
    _collections.insert("log-sez",action);
    connect(action, SIGNAL(triggered()),this, SLOT(updateSezLog()));
    sezBar->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    action->setIconText(tr("Services"));
    action->setToolTip(tr("Check Vulnerabilities"));
    action->setCheckable(true);
    _collections.insert("vuln-sez",action);
    connect(action, SIGNAL(triggered()),this, SLOT(updateSezVuln()));
    sezBar->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
    action->setIconText(tr("Discover"));
    action->setToolTip(tr("Network Discover"));
    action->setCheckable(true);
    _collections.insert("discover-sez",action);
    connect(action, SIGNAL(triggered()),this, SLOT(updateDiscoverVuln()));
    sezBar->addAction(action);

    sezBar->addSeparator();

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/show-menu.png")));
    action->setToolTip(tr("Enable/Disable Manual Parameters"));
    action->setCheckable(true);
    _collections.insert("par-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(parAdv()));
    sezBar->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    action->setIconText(tr("Nss Script"));
    action->setToolTip(tr("Enable/Disable NSS script"));
    action->setCheckable(true);
    _collections.insert("nss-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(NSSCheck()));
    sezBar->addAction(action);

    sezBar->setContextMenuPolicy(Qt::PreventContextMenu);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    action->setIconText(tr("Search"));
    action->setEnabled(false);
    _collections.insert("search-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(searchVulnNG()));
    toolBarSearch->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/left.png")));
    action->setIconText(tr("Back"));
    action->setEnabled(false);
    _collections.insert("back-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(tabWebBack()));
    toolBarSearch->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/right.png")));
    action->setIconText(tr("Forward"));
    action->setEnabled(false);
    _collections.insert("forward-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(tabWebForward()));
    toolBarSearch->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/button_cancel.png")));
    action->setIconText(tr("Stop"));
    action->setEnabled(false);
    _collections.insert("stop-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(tabWebStop()));
    toolBarSearch->addAction(action);

    toolBarSearch->setContextMenuPolicy(Qt::PreventContextMenu);

    action = new QAction(this);
    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/network-workgroup.png")));
    action->setToolTip(tr("Show/Hide Lookup"));
    action->setCheckable(true);
    _collections.insert("tab-look-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(updateTabLook()));
    toolBarTab->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/network-wired.png")));
    action->setToolTip(tr("Show/Hide Traceroot"));
    action->setCheckable(true);
    _collections.insert("tab-trace-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(updateTabTrace()));
    toolBarTab->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(QString::fromUtf8(":/images/images/utilities-system-monitor.png")));
    action->setToolTip(tr("Show/Hide Scan Monitor"));
    action->setCheckable(true);
    _collections.insert("tab-monitor-act",action);
    connect(action, SIGNAL(triggered()),this, SLOT(updateTabMonitor()));
    toolBarTab->addAction(action);
    
    toolBarTab->setContextMenuPolicy(Qt::PreventContextMenu);
}

void nmapClass::createToolButtonSetup()
{
    // new QToolButton menu
    menuSetup = new QToolButton();
    menuSetup->setPopupMode(QToolButton::InstantPopup);
    menuSetup->setText(tr("Setup"));
    menuSetup->setIcon(QIcon(QString::fromUtf8(":/images/images/tool.png")));
    
    QMenu *menu = new QMenu();
    menu->addAction(action_Scan_2);
    menu->addAction(actionSave);
    menu->addAction(actionSave_As_Menu);
    menu->addSeparator();
    menu->addAction(actionAdd_Bookmark);
    menu->addSeparator();
    menu->addAction(actionScan_section);
    menu->addAction(actionLog_section);
    menu->addAction(actionVulnerabilities_section);
    menu->addAction(actionSection_Discover);
    menu->addSeparator();
    menu->addAction(actionProfile);
    menu->addSeparator();
    menu->addMenu(menu_Help);
    menu->addSeparator();
    menu->addAction(actionMenuBar);
    menu->addSeparator();
    menu->addAction(action_Quit_2);
    menu->setLayoutDirection(Qt::LeftToRight);
    menuSetup->setMenu(menu);
    //toolBarQuit->addWidget(menuSetup);
    toolMenuBar->addWidget(menuSetup);
}


