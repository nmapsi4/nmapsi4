/*
    Copyright (C) 2011  Francesco Cecconi <francesco.cecconi@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


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

namespace pingInterface {
/*!
 * nping interface, main method for discovery section
 */
class mainDiscover : public QObject
{
    Q_OBJECT

public:
    /*!
     * Create a object for discovery Class
     */
    mainDiscover(int uid);
    ~mainDiscover();
    /*!
     * Return a QList of network interfaces
     */
    QList<QNetworkInterface> getAllInterfaces() const;
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

public slots:
    /*!
     * Check state of ip on the network (up/down) with nping QThread
     */
    void isUp(const QStringList networkIpList, QObject *parent, QStringList parameters);

protected:
    bool ipState;
    int uid_;
    QStringList m_ipSospended;
    QObject *m_parent;
    int ScanCounter;
    int threadLimit;
    QTimer *timer;
    bool connectState;
    QStringList parameters_;
    QList<QProcessThread*> _threadList;


private slots:
    /*!
     * Emit signal with nping QThread ByteArray output
     */
    void threadReturn(QStringList ipAddr, QByteArray ipBuffer, QByteArray BufferError);
    void repeatScanner();
    void stopDiscover();

signals:
    /*!
     * Return with a signal of ip state (up/down)
     */
    void endPing(QStringList ipAddr, bool state, const QByteArray callBuff);
};

}
#endif // MAINDISCOVER_H
