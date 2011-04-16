/***************************************************************************
 *   Copyright (C) 2008-2011 by Francesco Cecconi                          *
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

#include "../../mainwin.h"

void nmapClass::addMonitorHost(QTreeWidget* monitor, const QString host) 
{
    tabUi->setTabIcon(tabUi->indexOf(tabMainMonitor),QIcon(QString::fromUtf8(":/images/images/reload.png")));
    QTreeWidgetItem *hostThread = new QTreeWidgetItem(monitor);
    hostThread->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
    hostThread->setText(0, host);
    if(!frameAdv->isVisible()) {
        hostThread->setText(1,check_extensions().join(" "));
    } else {
        hostThread->setText(1,comboAdv->lineEdit()->text());
    }
    hostThread->setIcon(2, QIcon(QString::fromUtf8(":/images/images/reload.png")));
    hostThread->setText(2, "Scanning");
    monitorElem.push_front(hostThread);
    monitorElemHost.push_front(hostThread->text(0));
    monitorElemOptions.push_front(hostThread->text(1));
    monitorElemState.push_front(hostThread->text(2));
    updateScanCounter(1);
}


void nmapClass::delMonitorHost(QTreeWidget* monitor, const QString host) 
{
     for(int i=0; i < monitorElemHost.size(); i++) {
          if(monitorElemHost[i].endsWith(host)) {
#ifndef MAIN_NO_DEBUG
               qDebug() << "Monitor delete Scan::" << host;
#endif
               monitorElemHost.removeAt(i);
               monitorElemState.removeAt(i);
               monitorElemOptions.removeAt(i);
	       delete monitorElem.takeAt(i);
	       break; // remove only first elem
	  }
     }
     updateMonitorHost(monitor);
}

void nmapClass::updateMonitorHost(QTreeWidget* monitor) 
{
    memoryTools *memTools = new memoryTools();
    memTools->itemDeleteAll(monitorElem);
    delete memTools;
    monitor->clear();
    QTreeWidgetItem* item;

    for(int i=0; i < monitorElemHost.size(); i++) {
#ifndef MAIN_NO_DEBUG
	  qDebug() << "MONITOR::Elem Numeber:: " << monitorElemHost.size();
#endif
	  item = new QTreeWidgetItem(monitor);
          item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
	  item->setText(0, monitorElemHost[i]);
          item->setText(1, monitorElemOptions[i]);
          item->setIcon(2, QIcon(QString::fromUtf8(":/images/images/reload.png")));
          item->setText(2, monitorElemState[i]);
	  monitorElem.push_front(item);
     }

     QString title("Nmapsi4 ");

     if(monitorElemHost.size() == 0) {
         actionStop_Scan->setEnabled(false);
         tabUi->setTabIcon(tabUi->indexOf(tabMainMonitor),QIcon(QString::fromUtf8(":/images/images/utilities-system-monitor.png")));
     }

     updateScanCounter(0);   
}

void nmapClass::readFlowFromThread(const QString hostname, const QString lineData) 
{
    // read data line form thread
    // search hostname on treeWidget and update data rows (index = 2)
    // take only remaining time and remove character unused
    QString infoTmp_ = lineData.mid(lineData.indexOf("("),lineData.indexOf(")"));
    infoTmp_ = infoTmp_.remove('(');
    infoTmp_ = infoTmp_.remove(')');
    infoTmp_ = infoTmp_.remove('\n');
    // insert new information into monitor
    monitorElem[monitorElemHost.indexOf(hostname)]->setText(2,infoTmp_);
    monitorElemState[monitorElemHost.indexOf(hostname)] = infoTmp_;
}

void nmapClass::updateScanCounter(int type) 
{
    /*
     *   type 1: scan counter ++
     *   type 0: scan counter --
     */
    QString title("Nmapsi4 -");
    title.append(tr(" Active Scan "));
    title.append("(");

    if(type) {
        scanCounter++;
    } else {
        scanCounter--;
    }

    title.append(QString("%1").arg(scanCounter));
    title.append(")");
    setWindowTitle(title);
}

