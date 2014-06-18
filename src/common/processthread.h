/*
Copyright 2008-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QMetaType>
#include <QtCore/QPointer>
#include <QtCore/QDebug>

//local include
#include "debug.h"

class ProcessThread : public QThread
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
    ProcessThread(const QString& programName, const QStringList& parameters);
    ~ProcessThread();
signals:
    /*!
     * Return nmap QThread output with a Signal.
     */
    void threadEnd(const QStringList parameters, QByteArray dataBuffer, QByteArray errorBuffer);
    /*!
     * Return nmap QThread stdout for ETC and remaining scan time.
     */
    void flowFromThread(const QString parameters, QByteArray data);
    void dataIsReady(const QStringList parameters, QByteArray errorBuffer);

private:
    QByteArray m_pout;
    QByteArray m_perr;
    QStringList m_ParList;
    QString m_programName;
    QPointer<QProcess> m_process;

private slots:
    void readFinished();
    void stopProcess();
    void readyReadData();

protected:
    void run();
};

#endif
