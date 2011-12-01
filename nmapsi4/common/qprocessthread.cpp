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

QProcessThread::QProcessThread(const QString& programName, 
                               QByteArray& ProcB1, 
                               QByteArray& ProcB2, 
                               const QStringList& parameters)
     : _pout(ProcB1), 
       _perr(ProcB2),
       _ParList(parameters),
       _programName(programName)
{ 
}

QProcessThread::~QProcessThread()
{
#ifndef THREAD_NO_DEBUG
    qDebug() << "DEBUG:: ~QProcessThread()";
#endif
    stopProcess();
}

void QProcessThread::run() 
{     
     _process = new QProcess();
     qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

     connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)),
             this, SLOT(readFinished()));
     connect(_process, SIGNAL(readyReadStandardOutput()),
             this, SLOT(readyReadData()));

#ifndef THREAD_NO_DEBUG
     qDebug() << "DEBUG::ThreadString:: " << _ParList;
#endif
     _process->start(_programName, _ParList);
     
     exec();
     // emit signal, scan is end
     emit threadEnd(_ParList, _pout, _perr);

#ifndef THREAD_NO_DEBUG
     qDebug() << "THREAD::Stop(" << _programName << ")";
#endif
}

void QProcessThread::readFinished() 
{
#ifndef THREAD_NO_DEBUG
     qDebug() << "THREAD::Start(" << _programName << ")";
#endif
     // set scan return buffer
     _perr  = _process->readAllStandardError(); // read error buffer
     _process->close();
     delete _process;
     exit(0);
}

void QProcessThread::stopProcess()
{
    // stop scan process (Slot)
    if (!_process)
    {
        return;
    }

    if (_process->state() == QProcess::Running)
    {
        _process->close();
        delete _process;
    }
}

void QProcessThread::readyReadData() 
{
    // read realtime data from QProcess
    QByteArray realByte = _process->readAllStandardOutput();
    _pout.append(realByte);
    QString stream_(realByte);
    // emit signal for data trasmission to parent
    if (!stream_.isEmpty()) 
    {
        emit flowFromThread(_ParList[_ParList.size()-1], stream_);
    }
}
