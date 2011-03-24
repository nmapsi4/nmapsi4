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

#include "../mainwin.h"

void nmapClass::checkFullScreen()
{

    if (isFullScreen()) {
        setWindowState(windowState() & ~Qt::WindowFullScreen );
        actionFullS->setChecked(false);
    } else {
	setWindowState(windowState() | Qt::WindowFullScreen );
        actionFullS->setChecked(true);
    }
}

void nmapClass::updateMenuBar()
{

    if (menubar->isVisible()) {
        menubar->setVisible(false);
        actionMenuBar->setChecked(false);
    } else {
        menubar->setVisible(true);
        actionMenuBar->setChecked(true);
    }
}


void nmapClass::showMainToolBar()
{
    if (toolBar->isVisible()) {
        toolBar->setVisible(false);
        actionMain_Toolbars->setChecked(false);
    } else {
        toolBar->setVisible(true);
        actionMain_Toolbars->setChecked(true);
    }
}

void nmapClass::showActionToolBar()
{
    if (toolBar_2->isVisible()) {
        toolBar_2->setVisible(false);
        actionActions_Toolbar->setChecked(false);
    } else {
        toolBar_2->setVisible(true);
        actionActions_Toolbar->setChecked(true);
    }
}


void nmapClass::showStatusBar()
{
    if (statusbar->isVisible()) {
        statusbar->setVisible(false);
        actionShow_Status_Bar->setChecked(false);
    } else {
        statusbar->setVisible(true);
        actionShow_Status_Bar->setChecked(true);
    }
}

void nmapClass::optionListUpdate() {
  

    if (scanW->isSelected()) {
	stackedOptions->setCurrentIndex(0);
    } else if (toolW->isSelected()) {
	stackedOptions->setCurrentIndex(5);
    } else if (discoverW->isSelected()) {
	stackedOptions->setCurrentIndex(1);
    } else if (fileW->isSelected()) {
	stackedOptions->setCurrentIndex(2);
    } else if (timingW->isSelected()) {
	stackedOptions->setCurrentIndex(3);
    } else if (nseW->isSelected()) {
	stackedOptions->setCurrentIndex(6);
    }
}

void nmapClass::optionListCreate() {
  
    optionsListScan->setIconSize(QSize(52, 52));
  
    scanW = new QListWidgetItem(optionsListScan);
    scanW->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    scanW->setText(tr("Scan")); // scan Options
    
    toolW = new QListWidgetItem(optionsListScan);
    toolW->setIcon(QIcon(QString::fromUtf8(":/images/images/tool.png")));
    toolW->setText(tr("Options"));
    
    discoverW = new QListWidgetItem(optionsListScan);
    discoverW->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    discoverW->setText(tr("Discover"));
    
    fileW = new QListWidgetItem(optionsListScan);
    fileW->setIcon(QIcon(QString::fromUtf8(":/images/images/folder_open.png")));
    fileW->setText(tr("File"));
    
    timingW = new QListWidgetItem(optionsListScan);
    timingW->setIcon(QIcon(QString::fromUtf8(":/images/images/player_time.png")));
    timingW->setText(tr("Timing"));
    
    nseW = new QListWidgetItem(optionsListScan);
    nseW->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    nseW->setText(tr("Nse"));
    
    connect(optionsListScan, SIGNAL(itemSelectionChanged()),
	  this, SLOT(optionListUpdate()));
	
    scanW->setSelected(true);
}
