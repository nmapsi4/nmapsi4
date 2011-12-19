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

#ifndef DISCOVERMANAGER_H
#define DISCOVERMANAGER_H

#include <QtCore/QObject>
#include <QtGui/QTreeWidgetItem>

#include "discover.h"

class nmapClass;

class discoverManager : public QObject
{
    Q_OBJECT

public:
    discoverManager(nmapClass* parent);
    ~discoverManager();
    void defaultDiscoverProbes();

private:
    void updateSRdata();
    void discoveryClear();

    nmapClass* _ui;
    QList<discover*> listDiscover;
    QList<QTreeWidgetItem*> listTreeItemDiscover;
    int ipCounter;
    int _uid;
    QStringList sendList;
    QStringList recvList;
    bool _discoverIsActive;

signals:
    void killDiscover();


public slots:
    void startDiscover();
    void discoverIp(const QString& interface);
    void discoverIpState();
    void stopDiscover();
    void runtimeScanDiscover();
    void callScanDiscover();

private slots:
    void pingResult(const QStringList hostname, bool state, const QByteArray callBuff);
};

#endif // DISCOVERMANAGER_H
