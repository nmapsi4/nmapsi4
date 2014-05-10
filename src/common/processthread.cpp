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

#include "processthread.h"

ProcessThread::ProcessThread(const QString& programName, const QStringList& parameters)
    : m_ParList(parameters), m_programName(programName)
{
}

ProcessThread::~ProcessThread()
{
#ifndef THREAD_NO_DEBUG
    qDebug() << "DEBUG:: ~ProcessThread( " << m_programName << " )";
#endif
    stopProcess();
}

void ProcessThread::run()
{
    m_process = new QProcess();
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

    connect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(readFinished()));
    connect(m_process, SIGNAL(readyReadStandardOutput()),
            this, SLOT(readyReadData()));

#ifndef THREAD_NO_DEBUG
    qDebug() << "ProcessThread::Command:: " << m_ParList;
#endif

    m_process->start(m_programName, m_ParList);

    exec();
    // emit signal, scan is end
    emit threadEnd(m_ParList, m_pout, m_perr);
    emit dataIsReady(m_ParList, m_perr);
}

void ProcessThread::readFinished()
{
    // set scan return buffer
    m_perr  = m_process->readAllStandardError(); // read error buffer
    m_process->close();
    delete m_process;
    exit(0);
}

void ProcessThread::stopProcess()
{
    // stop scan process
    if (!m_process) {
        return;
    }

    if (m_process->state() == QProcess::Running) {
        //m_process.data()->close();
        // NOTE:: close() function segfault
        m_process->closeWriteChannel();
        m_process->closeReadChannel(m_process->readChannel());
        m_process->kill();
        m_process->waitForFinished();
        delete m_process;
    }
}

void ProcessThread::readyReadData()
{
    // read realtime data from QProcess
    QByteArray realtimeByteArray(m_process->readAllStandardOutput());
    if (!realtimeByteArray.isEmpty()) {
        m_pout.append(realtimeByteArray);
        // emit signal for data trasmission to parent
        emit flowFromThread(m_ParList[m_ParList.size() - 1], realtimeByteArray);
    }
}
