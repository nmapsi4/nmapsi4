/***************************************************************************
 *   Copyright (C) 2008-2009 by Francesco Cecconi                          *
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
     scanMonitor->setIconSize(QSize::QSize(32, 32));
     scanMonitor->header()->setResizeMode(0, QHeaderView::Interactive);
     QTreeWidgetItem *hostThread = new QTreeWidgetItem(monitor);
     hostThread->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
     hostThread->setText(0, host);
     hostThread->setText(1, tr("Starting..."));
     monitorElem.push_front(hostThread);
     monitorElemHost.push_front(hostThread->text(0));
     monitorElemState.push_front(hostThread->text(1));
     tabWidget->setCurrentIndex(2);
}


void nmapClass::delMonitorHost(QTreeWidget* monitor, QString host) {
     for(int i=0; i < monitorElemHost.size(); i++) {
          if(monitorElemHost[i].endsWith(host)) {
#ifndef MAIN_NO_DEBUG
               qDebug() << "Monitor delete Scan::" << host;
#endif
               monitorElemHost.removeAt(i);
               monitorElemState.removeAt(i);
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
#ifndef MAIN_NO_DEBUG
	  qDebug() << "MONITOR::Elem Numeber:: " << monitorElemHost.size();
#endif
	  item = new QTreeWidgetItem(monitor);
          item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
	  item->setText(0, monitorElemHost[i]);
	  item->setText(1, monitorElemState[i]);
	  monitorElem.push_front(item);
     }
     
}

