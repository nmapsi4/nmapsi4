/***************************************************************************
 *   Copyright (C) 2007-2008 by Francesco Cecconi                          *
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

void nmapClass::resize_tab_Options_Scan() {
     if(widgetOptions->height() == 0) {
	  widgetOptions->setGeometry(0,widgetOptions->y()-140,this->width(),151);
	  stackedWidget->setFixedWidth(this->width()-4);
	  groupBox->setFixedWidth(this->width()-17);
	  stackedWidget->setCurrentIndex(0);
     } else if(stackedWidget->currentIndex() != 0){
	  stackedWidget->setCurrentIndex(0);
	  stackedWidget->setFixedWidth(this->width()-4);
	  groupBox->setFixedWidth(this->width()-17);
     } else 
	  widgetOptions->setGeometry(0,widgetOptions->y()+140,this->width(),0); // close widget

}

void nmapClass::resize_tab_Options() {
 	
     if(widgetOptions->height() == 0) {
	  widgetOptions->setGeometry(0,widgetOptions->y()-140,this->width(),151);
	  stackedWidget->setFixedWidth(this->width()-4);
	  groupBox_2->setFixedWidth(this->width()-17);
	  stackedWidget->setCurrentIndex(6);
     } else if(stackedWidget->currentIndex() != 6){
	  stackedWidget->setCurrentIndex(6);
	  stackedWidget->setFixedWidth(this->width()-4);
	  groupBox_2->setFixedWidth(this->width()-17);
     } else 
	  widgetOptions->setGeometry(0,widgetOptions->y()+140,this->width(),0);
}

void nmapClass::resize_tab_MiscOptions() {
	if(widgetOptions->height() == 0) {
	    widgetOptions->setGeometry(0,widgetOptions->y()-140,this->width(),151);
	    stackedWidget->setFixedWidth(this->width()-4);
	    groupBox_3->setFixedWidth(this->width()-17);
	    stackedWidget->setCurrentIndex(1);
	} else if(stackedWidget->currentIndex() != 1){
	     stackedWidget->setCurrentIndex(1);
	     stackedWidget->setFixedWidth(this->width()-4);
	     groupBox_3->setFixedWidth(this->width()-17);
	} else 
	     widgetOptions->setGeometry(0,widgetOptions->y()+140,this->width(),0);
}

void nmapClass::resize_tab_Discover() {
     if(widgetOptions->height() == 0) {
	     widgetOptions->setGeometry(0,widgetOptions->y()-140,this->width(),151);
	     stackedWidget->setFixedWidth(this->width()-4);
	     groupBox_4->setFixedWidth(this->width()-17);
	     stackedWidget->setCurrentIndex(2);
	} else if(stackedWidget->currentIndex() != 2){
	     stackedWidget->setCurrentIndex(2);
	     stackedWidget->setFixedWidth(this->width()-4);
	     groupBox_4->setFixedWidth(this->width()-17);
	} else 
	     widgetOptions->setGeometry(0,widgetOptions->y()+140,this->width(),0);
}

void nmapClass::resize_tab_Files() {
     if(widgetOptions->height() == 0) {
	  widgetOptions->setGeometry(0,widgetOptions->y()-140,this->width(),151);
	  stackedWidget->setFixedWidth(this->width()-4);
	  groupBox_5->setFixedWidth(this->width()-17);
	  stackedWidget->setCurrentIndex(3);
     } else if(stackedWidget->currentIndex() != 3){
	  stackedWidget->setCurrentIndex(3);
	  stackedWidget->setFixedWidth(this->width()-4);
	  groupBox_5->setFixedWidth(this->width()-17);
     } else 
	  widgetOptions->setGeometry(0,widgetOptions->y()+140,this->width(),0);
}

void nmapClass::resize_tab_Timing() {
     if(widgetOptions->height() == 0) {
	  widgetOptions->setGeometry(0,widgetOptions->y()-140,this->width(),151);
	  stackedWidget->setFixedWidth(this->width()-4);
	  groupBox_6->setFixedWidth(this->width()-17);
	  stackedWidget->setCurrentIndex(4);
     } else if(stackedWidget->currentIndex() != 4){
	  stackedWidget->setCurrentIndex(4);
	  stackedWidget->setFixedWidth(this->width()-4);
	  groupBox_6->setFixedWidth(this->width()-17);
     } else 
	  widgetOptions->setGeometry(0,widgetOptions->y()+140,this->width(),0);
}

void nmapClass::checkFullScreen() {
    
     if(this->isFullScreen()) {
	  this->showNormal();
	  actionFullS->setChecked(false);
     } else {
	  this->showFullScreen();
	  actionFullS->setChecked(true);
     }
}

void nmapClass::updateMenuBar() {

     if(menubar->isVisible()) {
	  menubar->setVisible(false);
	  actionMenuBar->setChecked(false);
     } else {
	  menubar->setVisible(true);
	  actionMenuBar->setChecked(true);
     }
}


void nmapClass::showMainToolBar() {
     if(toolBar->isVisible()) {
	  toolBar->setVisible(false);
	  actionMain_Toolbars->setChecked(false);
     } else {
	  toolBar->setVisible(true);
	  actionMain_Toolbars->setChecked(true);
     }
}

void nmapClass::showActionToolBar() {
     if(toolBar_2->isVisible()) {
	  toolBar_2->setVisible(false);
	  actionActions_Toolbar->setChecked(false);
     } else {
	  toolBar_2->setVisible(true);
	  actionActions_Toolbar->setChecked(true);
     }
}


void nmapClass::showStatusBar() {
     if(statusbar->isVisible()) {
	  statusbar->setVisible(false);
	  actionShow_Status_Bar->setChecked(false);
     } else {
	  statusbar->setVisible(true);
	  actionShow_Status_Bar->setChecked(true);
     }
}
