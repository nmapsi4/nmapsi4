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

#ifndef DISCOVERMANAGER_H
#define DISCOVERMANAGER_H

#include <QtCore/QObject>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QSplitter>

#include "discover.h"
#include "regularexpression.h"

class MainWindow;

class DiscoverManager : public QObject
{
    Q_OBJECT

public:
    DiscoverManager(MainWindow* parent=0);
    ~DiscoverManager();
    void defaultDiscoverProbes();
    void syncSettings();

private:
    void discoveryClear();

    MainWindow* m_ui;
    QList<Discover*> m_listDiscover;
    QList<QTreeWidgetItem*> m_listTreeItemDiscover;
    QList<QTreeWidgetItem*> m_listTreePackets;
    int m_ipCounter;
    int m_uid;
    bool m_discoverIsActive;
    QSplitter *m_discoverHorizontalSplitter;
    QSplitter *m_discoverVerticalSplitter;

signals:
    void killDiscoverFromIpsRange();
    void killDiscoverFromCIDR();

public slots:
    void startDiscover();
    void discoverIp(const QString& interface);
    void startDiscoverIpsFromRange();
    void stopDiscoverFromIpsRange();
    void stopDiscoverFromCIDR();
    void runtimeScanDiscover();
    void scanSingleDiscoveredIp();
    void scanAllDiscoveredIps();
    void startDiscoverIpsFromCIDR();
    void calculateAddressFromCIDR();

private slots:
    void endDiscoverIpsFromRange(const QStringList hostname, bool state, const QByteArray callBuff);
    void endDiscoverIpsFromCIDR();
    void currentDiscoverIpsFromCIDR(const QString parameters, const QString data);
};

#endif // DISCOVERMANAGER_H
