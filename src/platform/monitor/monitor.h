/***************************************************************************
 *   Copyright (C) 2011-2012 by Francesco Cecconi                          *
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

#include "ui_monitorwidget.h"

// Qt include
#include <QtGui/QTreeWidget>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QPair>
#include <QtCore/QWeakPointer>
#include <QtCore/QSettings>
#include <QtNetwork/QHostInfo>

#ifdef Q_WS_X11
#include <QtDBus/QDBusConnection>
#endif

// local include
#include "memorytools.h"
#include "processthread.h"
#include "monitorhostscandetails.h"
#include "lookupmanager.h"
#include "digmanager.h"

using namespace memory;

class MainWindow;

class MonitorWidget : public QWidget, public Ui::MonitorWidgetForm
{
    Q_OBJECT
public:
    explicit MonitorWidget(QWidget* parent = 0);
};

class Monitor : public QObject
{
    Q_OBJECT

#ifdef Q_WS_X11
    Q_CLASSINFO("D-Bus Interface", "org.nmapsi4.Nmapsi4")
#endif

public:
    Monitor(MainWindow* parent);
    ~Monitor();

    enum LookupType
    {
        DisabledLookup,
        InternalLookup,
        DigLookup
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
    /*
     * Load max parallel scan option from config file
     */
    void updateMaxParallelScan();

    MonitorWidget* m_monitorWidget;

private:
    void startScan(const QString hostname, QStringList parameters);
    void startLookup(const QString hostname, LookupType option);
    void updateMonitorHost(const QString hostName, int valueIndex, const QString newData);
    /*
     * This method remove scanThread elem from scan hashTable
     */
    ProcessThread* takeMonitorElem(const QString hostName);
    /*
     * Delete host from monitor
     */
    void delMonitorHost(const QString hostName);
    /*
     * Cache for parallel host thread
     */
    void cacheScan(const QString& hostname, const QStringList& parameters, LookupType option, QTreeWidgetItem *item);

    QList<QTreeWidgetItem*> m_monitorElem;
    QList<LookupManager*> m_internealLookupList;
    QList<DigManager*> m_digLookupList;
    QList< QPair<QString, QStringList> > m_hostScanCacheList;
    QList< QPair<LookupType, QTreeWidgetItem*> > m_lookupScanCacheList;
    QHash<QString, ProcessThread*> m_scanHashList;
    QHash<QString, QStringList> m_scanHashListFlow;
    QHash<QString, int> m_hostIdList;
    MainWindow* m_ui;
    int m_parallelThreadLimit;
    int m_idCounter;
    bool m_isHostcached;
    QTimer* m_timer;

signals:
    /*
     * Exported with dbus
     */
    Q_SCRIPTABLE void monitorUpdated(int hostNumber);
    /*
     * Return scan result
     */
    void hostFinisced(const QStringList parametersList, QByteArray dataBuffer, QByteArray errorBuffer, int id);

private slots:
    void readFlowFromThread(const QString hostname, QString lineData);
    void scanFinisced(const QStringList parametersList, QByteArray dataBuffer, QByteArray errorBuffer);
    void lookupFinisced(QHostInfo info, int state, const QString hostname);
    void cacheRepeat();
    /*
     * Stop host scan selected in the QTreeWidget.
     */
    void stopSelectedScan();
    void stopAllScan();
    void showSelectedScanDetails();
    void monitorRuntimeEvent();
};

#endif
