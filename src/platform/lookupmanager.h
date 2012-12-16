/*
Copyright 2009-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LOOKUPMANAGER_H
#define LOOKUPMANAGER_H

#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtNetwork/QHostInfo>

//local include
#include "debug.h"

/*!
 * Internal lookup thread class, return QHostInfo with
 * address resolv.
 */

class LookupManager : public QThread
{
    Q_OBJECT

public:
    /*!
     * Create a QThread and start static QHostInfo resolv.
     */
    LookupManager(const QString hostname);
    ~LookupManager();

signals:
    /*!
     * Return QHostInfo with a signal with address resolv.
     */
    void threadEnd(QHostInfo, int, const QString);

private:
    QString m_host;
    QHostInfo m_info;
    QObject* m_par;

private slots:
    /*!
     * Stop static QHostInfo resolv immediately.
     */
    void killLookup();

protected:
    void run();
};

#endif
