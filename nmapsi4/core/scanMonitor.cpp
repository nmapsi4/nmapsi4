/***************************************************************************
 *   Copyright (C) 2008 by Francesco Cecconi                          *
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



void nmapClass::addMonitorHost(QTreeWidget* monitor, QString host) {
     // TODO
     QTreeWidgetItem *hostThread = new QTreeWidgetItem(monitor);
     //monitorElem.push_front(hostThread);
     hostThread->setText(0, host);
     monitorElem.push_front(hostThread->text(0));
}


void nmapClass::delMonitorHost(QTreeWidget* monitor, QString host) {
     // TODO
     // TODO clear a item deleted

     for(int i=0; i < monitorElem.size(); i++) {
	  if(monitorElem[i].contains(host)) {
	       // TODO take a treeItem and clear a memory
	       // URGENT::: memory leack
	       qDebug() << "Monitor delete Scan";
	       monitorElem.removeOne(monitorElem[i]);
	       break; // remove only first elem
	  }
     }
     updateMonitorHost(monitor);
}

void nmapClass::updateMonitorHost(QTreeWidget* monitor) {

     monitor->clear();
     QTreeWidgetItem* item;

     for(int i=0; i < monitorElem.size(); i++) {
	  qDebug() << "MONITOR::Elem Numeber:: " << monitorElem.size();
	  item = new QTreeWidgetItem(monitor);
	  item->setText(0, monitorElem[i]);
     }
     
}
