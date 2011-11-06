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

// local include
#include "../lib/memorytools.h"

using namespace memory;

class monitor : public QObject
{
    Q_OBJECT
    
    friend class nmapClass;
    
public:
    monitor();
    ~monitor();
    void addMonitorHost(QTreeWidget* monitor, const QString host,const QString paramter);
    void delMonitorHost(QTreeWidget* monitor, const QString host);
    bool searchMonitorElem(const QString hostname);
    int monitorHostNumber();
    
private:
    void updateMonitorHost(QTreeWidget* monitor);
    
protected:
    QList<QTreeWidgetItem*> monitorElem;
    QList<QString> monitorElemHost;
    QList<QString> monitorElemState;
    QList<QString> monitorElemOptions;
    
signals:
    void monitorUpdated(int hostNumber);
};

#endif