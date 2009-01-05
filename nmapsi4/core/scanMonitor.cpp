/***************************************************************************
 *   Copyright (C) 2008 by Francesco Cecconi                               *
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
     QTreeWidgetItem *hostThread = new QTreeWidgetItem(monitor);
     hostThread->setText(0, host);
     hostThread->setText(1, tr("Starting..."));
     monitorElem.push_front(hostThread);
     monitorElemHost.push_front(hostThread->text(0));
     monitorElemState.push_front(hostThread->text(1));
}


void nmapClass::delMonitorHost(QTreeWidget* monitor, QString host) {
     for(int i=0; i < monitorElemHost.size(); i++) {
	  if(monitorElemHost[i].contains(host)) {
	       qDebug() << "Monitor delete Scan";
	       monitorElemHost.removeOne(monitorElemHost[i]);
	       monitorElemState.removeOne(monitorElemState[i]);
	       delete monitorElem.takeAt(i);
	       break; // remove only first elem
	  }
     }
     updateMonitorHost(monitor);
}

void nmapClass::updateMonitorHost(QTreeWidget* monitor) {

     qDeleteAll(monitorElem);
     monitorElem.clear();
     monitor->clear();
     QTreeWidgetItem* item;

     for(int i=0; i < monitorElemHost.size(); i++) {
	  qDebug() << "MONITOR::Elem Numeber:: " << monitorElemHost.size();
	  item = new QTreeWidgetItem(monitor);
	  item->setText(0, monitorElemHost[i]);
	  item->setText(1, monitorElemState[i]);
	  monitorElem.push_front(item);
     }
     
}

