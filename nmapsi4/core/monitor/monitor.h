/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               * 
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

#ifndef MONITOR_H
#define MONITOR_H

// Qt include
#include <QtCore/QObject>
#include <QtGui/QTreeWidget>
#include <QtCore/QList>
#include <QtDBus/QDBusConnection>

// local include
#include "../lib/memorytools.h"

using namespace memory;

class monitor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.nmapsi4.Nmapsi4")
    
public:
    monitor(QTreeWidget* monitor);
    ~monitor();
    void addMonitorHost(const QString hostName,const QString parameter);
    void delMonitorHost(const QString hostName);
    void updateMonitorHost(const QString hostName, int valueIndex, const QString newData);
    bool searchMonitorElem(const QString hostname);
    int monitorHostNumber();
        
protected:
    QList<QTreeWidgetItem*> monitorElem;
    QTreeWidget* _monitor;
    
signals:
    void monitorUpdated(int hostNumber);
};

#endif
