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
#include <QtGui/QTreeWidget>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>

#ifdef Q_WS_X11
#include <QtDBus/QDBusConnection>
#endif

// local include
#include "memorytools.h"
#include "qprocessthread.h"
#include "details.h"
#include "lookupmanager.h"
#include "digmanager.h"

using namespace memory;

class nmapClass;

class monitor : public QObject
{
    Q_OBJECT

#ifdef Q_WS_X11
    Q_CLASSINFO("D-Bus Interface", "org.nmapsi4.Nmapsi4")
#endif

public:
    monitor(QTreeWidget* monitor, nmapClass* parent);
    ~monitor();

    enum LookupType
    {
        DigLookup,
        InternalLookup,
        DisabledLookup
    };
    /*
     * Add host in the monitor and start scan.
     */
    void addMonitorHost(const QString hostName, const QStringList parameters, LookupType option);
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

private:
    void startScan(const QString hostname, QStringList parameters);
    void startLookup(const QString hostname, LookupType option);
    void updateMonitorHost(const QString hostName, int valueIndex, const QString newData);
    /*
     * This method remove scanThread elem from scan hashTable
     */
    QProcessThread* takeMonitorElem(const QString hostName);
    /*
     * Delete host from monitor
     */
    void delMonitorHost(const QString hostName);

protected:
    QList<QTreeWidgetItem*> monitorElem;
    QHash<QString, QProcessThread*> _scanHashList;
    QHash<QString, QStringList> _scanHashListFlow;
    QList<lookupManager*> internealLookupList;
    QList<digManager*> digLookupList;
    QTreeWidget* _monitor;
    nmapClass* _ui;

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
    void lookupFinisced(QHostInfo info, int state, const QString hostname);

public slots:
        /*
     * Stop host scan selected in the QTreeWidget.
     */
    void stopSelectedScan();
    void showSelectedScanDetails();
};

#endif
