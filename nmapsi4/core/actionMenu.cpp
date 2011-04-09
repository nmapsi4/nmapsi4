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
    // QToolBar asction
    scanSez = new QAction(this);
    logSez = new QAction(this);
    vulnSez = new QAction(this);
    discoverSez = new QAction(this);
    nssAct = new QAction(this);
    parAct = new QAction(this);
    
    vulnSez->setCheckable(true);
    scanSez->setCheckable(true);
    logSez->setCheckable(true);
    discoverSez->setCheckable(true);

    actSearch = new QAction(this);
    actBack = new QAction(this);
    actForward = new QAction(this);
    actStop = new QAction(this);

    actTabLook = new QAction(this);
    actTabTrace = new QAction(this);
    actTabMonitor = new QAction(this);

    // Section QAction
    scanSez->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    scanSez->setIconText(tr("Scan"));
    scanSez->setToolTip(tr("Scan host(s)"));

    logSez->setIcon(QIcon(QString::fromUtf8(":/images/images/book.png")));
    logSez->setIconText(tr("Log"));
    logSez->setToolTip(tr("Scan Log"));

    vulnSez->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    vulnSez->setIconText(tr("Services"));
    vulnSez->setToolTip(tr("Check Vulnerabilities"));
    
    discoverSez->setIcon(QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
    discoverSez->setIconText(tr("Discover"));
    discoverSez->setToolTip(tr("Network Discover"));

    nssAct->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    nssAct->setIconText(tr("Nss Script"));
    nssAct->setToolTip(tr("Enable/Disable NSS script"));
    nssAct->setCheckable(true);

    parAct->setIcon(QIcon(QString::fromUtf8(":/images/images/show-menu.png")));
    parAct->setToolTip(tr("Enable/Disable Manual Parameters"));
    parAct->setCheckable(true);

    sezBar->addAction(scanSez);
    sezBar->addAction(logSez);
    sezBar->addAction(vulnSez);
    sezBar->addAction(discoverSez);
    sezBar->addSeparator();
    sezBar->addAction(parAct);
    sezBar->addAction(nssAct);
    sezBar->setContextMenuPolicy(Qt::PreventContextMenu);

    actSearch->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    actSearch->setIconText(tr("Search"));
    actSearch->setEnabled(false);

    actBack->setIcon(QIcon(QString::fromUtf8(":/images/images/left.png")));
    actBack->setIconText(tr("Back"));
    actBack->setEnabled(false);

    actForward->setIcon(QIcon(QString::fromUtf8(":/images/images/right.png")));
    actForward->setIconText(tr("Forward"));
    actForward->setEnabled(false);

    actStop->setIcon(QIcon(QString::fromUtf8(":/images/images/button_cancel.png")));
    actStop->setIconText(tr("Stop"));
    actStop->setEnabled(false);

    toolBarSearch->addAction(actSearch);
    toolBarSearch->addAction(actBack);
    toolBarSearch->addAction(actForward);
    toolBarSearch->addAction(actStop);
    toolBarSearch->setContextMenuPolicy(Qt::PreventContextMenu);

    actTabLook->setIcon(QIcon(QString::fromUtf8(":/images/images/network-workgroup.png")));
    actTabLook->setToolTip(tr("Show/Hide Lookup"));
    actTabLook->setCheckable(true);

    actTabTrace->setIcon(QIcon(QString::fromUtf8(":/images/images/network-wired.png")));
    actTabTrace->setToolTip(tr("Show/Hide Traceroot"));
    actTabTrace->setCheckable(true);

    actTabMonitor->setIcon(QIcon(QString::fromUtf8(":/images/images/utilities-system-monitor.png")));
    actTabMonitor->setToolTip(tr("Show/Hide Scan Monitor"));
    actTabMonitor->setCheckable(true);
    
    toolBarTab->addAction(actTabLook);
    toolBarTab->addAction(actTabTrace);
    toolBarTab->addAction(actTabMonitor);
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
    menu->addSeparator();
    menu->addAction(actionProfile);
    menu->addSeparator();
    menu->addMenu(menu_Help);
    menu->addSeparator();
    menu->addAction(actionMenuBar);
    menu->addSeparator();
    menu->addAction(action_Quit_2);
    
    menuSetup->setMenu(menu);
    //toolBarQuit->addWidget(menuSetup);
    toolMenuBar->addWidget(menuSetup);
}


