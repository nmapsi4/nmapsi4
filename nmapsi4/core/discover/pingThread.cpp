/***************************************************************************
 *   Copyright (C) 2009-2011 by Francesco Cecconi                          *
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

#include "pingThread.h"

pingThread::pingThread(QByteArray& ProcB1, const QStringList hostname, QObject *parent)
     : m_pout(ProcB1), 
       m_host(hostname),
       m_par(parent)
{
    // TODO: port parent
    /*connect(m_par, SIGNAL(killScan()),
            this, SLOT(stopProcess()));*/

}

void pingThread::run() 
{
    m_proc = new QProcess();
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    connect(m_proc, SIGNAL(finished(int, QProcess::ExitStatus)), 
	    this, SLOT(setValue()));

    m_proc->start("nping", m_host);
     
    exec();
    emit threadEnd(m_host, m_pout, this);
 }

void pingThread::setValue() 
{
    m_pout  = m_proc->readAllStandardOutput(); // read std buffer
    delete m_proc;
    exit(0);
}

void pingThread::stopProcess() 
{
     if(m_proc->state() == QProcess::Running) {
	  m_proc->terminate();
     }
}
