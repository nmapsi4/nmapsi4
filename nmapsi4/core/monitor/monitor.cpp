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

#include "monitor.h"

monitor::monitor()
{
}

monitor::~monitor()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(monitorElem);
}

bool monitor::searchMonitorElem(const QString hostname)
{
    return monitorElemHost.contains(hostname) ? true : false;
}

int monitor::monitorHostNumber()
{
    return monitorElemHost.size();
}

void monitor::addMonitorHost(QTreeWidget* monitor, const QString host, const QString paramter) 
{
    QTreeWidgetItem *hostThread = new QTreeWidgetItem(monitor);
    hostThread->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
    hostThread->setText(0, host);
    hostThread->setText(1,paramter);
    hostThread->setIcon(2, QIcon(QString::fromUtf8(":/images/images/reload.png")));
    hostThread->setText(2, "Scanning");
    monitorElem.push_front(hostThread);
    monitorElemHost.push_front(hostThread->text(0));
    monitorElemOptions.push_front(hostThread->text(1));
    monitorElemState.push_front(hostThread->text(2));

    emit monitorUpdated();
}


void monitor::delMonitorHost(QTreeWidget* monitor, const QString host) 
{
     for(int i=0; i < monitorElemHost.size(); i++) 
     {
          if(monitorElemHost[i].endsWith(host)) 
          {
              monitorElemHost.removeAt(i);
              monitorElemState.removeAt(i);
              monitorElemOptions.removeAt(i);
              delete monitorElem.takeAt(i);
              // FIXME:: remove break with duplicate check
              break; // remove only first elem
           }
     }
     
     updateMonitorHost(monitor);
     emit monitorUpdated();
}

void monitor::updateMonitorHost(QTreeWidget* monitor)
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(monitorElem);
    monitor->clear();
    QTreeWidgetItem* item;

    for (int i=0; i < monitorElemHost.size(); i++)
    {
        item = new QTreeWidgetItem(monitor);
        item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
        item->setText(0, monitorElemHost[i]);
        item->setText(1, monitorElemOptions[i]);
        item->setIcon(2, QIcon(QString::fromUtf8(":/images/images/reload.png")));
        item->setText(2, monitorElemState[i]);
        monitorElem.push_front(item);
    }
}
