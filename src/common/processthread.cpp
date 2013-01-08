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

    connect(m_process.data(), SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(readFinished()));
    connect(m_process.data(), SIGNAL(readyReadStandardOutput()),
            this, SLOT(readyReadData()));

#ifndef THREAD_NO_DEBUG
    qDebug() << "ProcessThread::Command:: " << m_ParList;
#endif

    m_process.data()->start(m_programName, m_ParList);

    exec();
    // emit signal, scan is end
    emit threadEnd(m_ParList, m_pout, m_perr);
    emit dataIsReady(m_ParList, m_perr);
}

void ProcessThread::readFinished()
{
    // set scan return buffer
    m_perr  = m_process.data()->readAllStandardError(); // read error buffer
    m_process.data()->close();
    delete m_process.data();
    exit(0);
}

void ProcessThread::stopProcess()
{
    // stop scan process
    if (m_process.isNull()) {
        return;
    }

    if (m_process.data()->state() == QProcess::Running) {
        //m_process.data()->close();
        // NOTE:: close() function segfault
        m_process.data()->closeWriteChannel();
        m_process.data()->closeReadChannel(m_process.data()->readChannel());
        m_process.data()->kill();
        m_process.data()->waitForFinished();
        delete m_process.data();
    }
}

void ProcessThread::readyReadData()
{
    // read realtime data from QProcess
    QByteArray realByte(m_process.data()->readAllStandardOutput());
    if (!realByte.isEmpty()) {
        m_pout.append(realByte);
        // emit signal for data trasmission to paren
        emit flowFromThread(m_ParList[m_ParList.size() - 1], realByte);
    }
}
