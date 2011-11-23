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
#include <QtCore/QHash>

// local include
#include "memorytools.h"
#include "scanThread.h"
#include "details.h"

using namespace memory;

class monitor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.nmapsi4.Nmapsi4")
    
public:
    monitor(QTreeWidget* monitor, QObject* parent);
    ~monitor();
    /*
     * Add host in the monitor and start scan.
     */
    void addMonitorHost(const QString hostName,const QStringList parameters);
    /*
     * Return true if host is present in the monitor, otherwise return false.
     */
    bool isHostOnMonitor(const QString hostname);
    /*
     * Return current number of scanning host in the monitor.
     */
    int monitorHostNumber();
    /*
     * Clear all host in monitor
     */
    void clearHostMonitor();
    /*
     * Clear all scan host details
     */
    void clearHostMonitorDetails();
    /*
     * Stop host scan selected in the QTreeWidget.
     */
    void stopSelectedScan();
    void showSelectedScanDetails();
    
private:
    void startScan(const QString hostname, QStringList parameters);
    void updateMonitorHost(const QString hostName, int valueIndex, const QString newData);
    /*
     * This method remove scanThread elem from scan hashTable
     */
    scanThread* takeMonitorElem(const QString hostName);
    /*
     * Delete host from monitor
     */
    void delMonitorHost(const QString hostName);
        
protected:
    QList<QTreeWidgetItem*> monitorElem;
    QHash<QString, scanThread*> _scanHashList;
    QHash<QString, QStringList> _scanHashListFlow;
    QTreeWidget* _monitor;
    QObject* _parent;
    
signals:
    /*
     * Exported with dbus
     */
    Q_SCRIPTABLE void monitorUpdated(int hostNumber);
    /*
     * Return scan result
     */
    void hostFinisced(const QStringList parametersList, QByteArray dataBuffer, QByteArray errorBuffer);
    
private slots:
    void readFlowFromThread(const QString hostname, QString lineData);
    void scanFinisced(const QStringList parametersList, QByteArray dataBuffer, QByteArray errorBuffer);
};

#endif
