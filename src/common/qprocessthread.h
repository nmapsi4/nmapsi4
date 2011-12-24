/***************************************************************************
 *   Copyright (C) 2008-2011 by Francesco Cecconi                          *
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

#ifndef QPROCESSTHREAD_H
#define QPROCESSTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QMetaType>
#include <QtCore/QWeakPointer>
#include <QtCore/QDebug>

//local include
#include "nmapsi4Debug.h"

class QProcessThread : public QThread
{
    /*!
    * nmap thread class, start nmap with a QProcess and return
    * QByteArray result with a signal.
    */
    Q_OBJECT

public:
    /*!
     * Create a nmap QThread and start QProcess for nmap
     * with parameters.
     */
    QProcessThread(const QString& programName, const QStringList& parameters);
    ~QProcessThread();
signals:
    /*!
     * Return nmap QThread output with a Signal.
     */
    void threadEnd(const QStringList parameters, QByteArray dataBuffer, QByteArray errorBuffer);
    /*!
     * Return nmap QThread stdout for ETC and remaining scan time.
     */
    void flowFromThread(const QString parameters, const QString data);

private:
    QByteArray m_pout;
    QByteArray m_perr;
    QStringList m_ParList;
    QString m_programName;
    QWeakPointer<QProcess> m_process;

private slots:
    void readFinished();
    void stopProcess();
    void readyReadData();

protected:
    void run();
};

#endif
