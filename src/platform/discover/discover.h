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
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostInfo>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QPointer>
#include <QtCore/QTimer>
// local include
#include "qprocessthread.h"

class discover : public QObject
{
    Q_OBJECT

public:
    enum InterfaceOption
    {
        AllInterfaceWithAddress,
        AllInterface
    };

    /*!
     * Create a object for discovery Class
     */
    discover(int uid);
    ~discover();
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

private:
    void isUp(const QString networkIp, QObject *parent, QStringList parameters);

    bool m_ipState;
    bool m_connectState;
    int m_uid;
    int m_threadLimit;
    QStringList m_ipSospended;
    QStringList m_parameters;
    QList<QProcessThread*> m_threadList;
    QObject* m_parent;
    QTimer* m_timer;

public slots:
    /*!
     * Check state of ip on the network (up/down) with nping QThread
     */
    void isUp(const QStringList networkIpList, QObject *parent, QStringList parameters);

private slots:
    /*!
     * Emit signal with nping QThread ByteArray output
     */
    void threadReturn(const QStringList ipAddr, QByteArray ipBuffer, QByteArray BufferError);
    void repeatScanner();
    void stopDiscover();

signals:
    /*!
     * Return with a signal of ip state (up/down)
     */
    void endPing(const QStringList ipAddr, bool state, const QByteArray callBuff);
};

#endif // MAINDISCOVER_H
