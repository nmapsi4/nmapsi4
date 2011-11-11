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
#include "nmapsi4adaptor.h"

monitor::monitor(QTreeWidget* monitor) : _monitor(monitor)
{
    new Nmapsi4Adaptor(this);
    // FIXME: with full mode It is registrered into root dbus session
    QDBusConnection dbus = QDBusConnection::sessionBus();
//     if (!dbus.isConnected())
//     {
//         dbus = QDBusConnection::systemBus();
//     }
    dbus.registerObject("/Nmapsi4",this);
    bool value = dbus.registerService("org.nmapsi4.Nmapsi4");

    if (!value)
    {
        qDebug() << "DBUS:: error with dbus connection";
    }
}

monitor::~monitor()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(monitorElem);
}

bool monitor::searchMonitorElem(const QString hostname)
{
    QList<QTreeWidgetItem*>::const_iterator i;
    for (i = monitorElem.begin(); i != monitorElem.end(); ++i)
    {
        if ((*i)->text(0) == hostname)
        {
            return true;
        }
    }
    
    return false;
}

int monitor::monitorHostNumber()
{
    return monitorElem.size();
}

void monitor::addMonitorHost(const QString hostName, const QStringList parameters)
{
    QTreeWidgetItem *hostThread = new QTreeWidgetItem(_monitor);
    hostThread->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
    hostThread->setText(0, hostName);
    hostThread->setText(1, parameters.join(" "));
    hostThread->setIcon(2, QIcon(QString::fromUtf8(":/images/images/reload.png")));
    hostThread->setText(2, "Scanning");
    monitorElem.push_front(hostThread);

    emit monitorUpdated(monitorHostNumber());
}


void monitor::delMonitorHost(const QString hostName) 
{
     for(int i=0; i < monitorElem.size(); i++) 
     {
          if(monitorElem[i]->text(0) == hostName) 
          {
              delete monitorElem.takeAt(i);
              // FIXME:: remove break with duplicate check
              break; // remove only first elem
           }
     }
     
     emit monitorUpdated(monitorHostNumber());
}

void monitor::updateMonitorHost(const QString hostName, int valueIndex, const QString newData)
{
    Q_ASSERT(valueIndex < _monitor->columnCount());
    
    QList<QTreeWidgetItem*>::const_iterator i;
    for (i = monitorElem.begin(); i != monitorElem.end(); ++i)
    {
        if ((*i)->text(0) == hostName)
        {
            (*i)->setText(valueIndex,newData);
        }
        
        break;
    }
}
