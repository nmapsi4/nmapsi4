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

#ifndef MAINDISCOVER_H
#define MAINDISCOVER_H

#include <QtCore/QList>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QWeakPointer>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostInfo>
// local include
#include "processthread.h"
#include "memorytools.h"

class DiscoverManager;

class Discover : public QObject
{
    Q_OBJECT

public:
    enum InterfaceOption
    {
        AllInterfaceWithAddress,
        AllInterface
    };

    enum IpProtocolType
    {
        IPv4,
        IPv6
    };

    /*!
     * Create a object for discovery Class
     */
    Discover(int uid);
    ~Discover();
    /*!
     * Return a QList of network interfaces
     */
    QList<QNetworkInterface> getAllInterfaces(InterfaceOption option) const;
    /*!
     * Return ip address for a QNetworkInterface
     */
    QList<QNetworkAddressEntry> getAddressEntries(QNetworkInterface interface) const;
    /*!
     * Return ip address for a interface name
     */
    QList<QNetworkAddressEntry> getAddressEntries(const QString interfaceName) const;
    /*!
     * Check state of ip on the network (up/down) with nping QThread
     */
    void fromList(const QStringList networkIpList, DiscoverManager *parent, QStringList parameters);
    void fromCIDR(const QString networkCIDR, QStringList parameters, DiscoverManager* parent, IpProtocolType type);

private:
    void fromList(const QString networkIp, DiscoverManager *parent, QStringList parameters);

    bool m_ipState;
    bool m_connectState;
    int m_uid;
    int m_threadLimit;
    QStringList m_ipSospended;
    QStringList m_parameters;
    QList<ProcessThread*> m_threadList;
    DiscoverManager* m_parent;
    QTimer* m_timer;

private slots:
    /*!
     * Emit signal with nping QThread ByteArray output
     */
    void fromListReturn(const QStringList ipAddr, QByteArray ipBuffer, QByteArray BufferError);
    void repeatScanner();
    void stopDiscoverFromList();
    void stopDiscoverFromCIDR();
    void currentCIDRValue(const QString parameters, const QString data);
    void endCIDR(const QStringList ipAddr, QByteArray ipBuffer, QByteArray bufferError);

signals:
    /*!
     * Return with a signal of ip state (up/down)
     */
    void fromListFinisched(const QStringList ipAddr, bool state, const QByteArray callBuff);
    void cidrFinisced(const QStringList ipAddr, QByteArray ipBuffer, QByteArray bufferError);
    void cidrCurrentValue(const QString parameters, const QString data);
};

#endif // MAINDISCOVER_H
