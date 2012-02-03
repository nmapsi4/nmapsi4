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

#include "qprocessthread.h"

QProcessThread::QProcessThread(const QString& programName, const QStringList& parameters)
: m_ParList(parameters), m_programName(programName)
{
}

QProcessThread::~QProcessThread()
{
#ifndef THREAD_NO_DEBUG
    qDebug() << "DEBUG:: ~QProcessThread( " << m_programName << " )";
#endif
    stopProcess();
}

void QProcessThread::run()
{
     m_process = new QProcess();
     qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

     connect(m_process.data(), SIGNAL(finished(int,QProcess::ExitStatus)),
             this, SLOT(readFinished()));
     connect(m_process.data(), SIGNAL(readyReadStandardOutput()),
             this, SLOT(readyReadData()));

#ifndef THREAD_NO_DEBUG
     qDebug() << "QProcessThread::Command:: " << m_ParList;
#endif

     m_process.data()->start(m_programName, m_ParList);

     exec();
     // emit signal, scan is end
     emit threadEnd(m_ParList, m_pout, m_perr);
}

void QProcessThread::readFinished()
{
     // set scan return buffer
     m_perr  = m_process.data()->readAllStandardError(); // read error buffer
     m_process.data()->close();
     delete m_process.data();
     exit(0);
}

void QProcessThread::stopProcess()
{
    // stop scan process (Slot)
    if (m_process.isNull())
    {
        return;
    }

    if (m_process.data()->state() == QProcess::Running)
    {
        m_process.data()->close();
        delete m_process.data();
    }
}

void QProcessThread::readyReadData()
{
    // read realtime data from QProcess
    QByteArray realByte = m_process.data()->readAllStandardOutput();
    m_pout.append(realByte);
    QString stream_(realByte);
    // emit signal for data trasmission to parent
    if (!stream_.isEmpty())
    {
        emit flowFromThread(m_ParList[m_ParList.size()-1], stream_);
    }
}
