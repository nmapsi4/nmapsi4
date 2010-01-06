/***************************************************************************
 *   Copyright (C) 2009-2010 by Francesco Cecconi                          *
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

#include "digThread.h"

digThread::digThread(QByteArray& ProcB1, const QStringList hostname, QObject *parent)
     : pout(ProcB1), 
       host(hostname),
       par(parent)
{

}

void digThread::run() {
     
     proc = new QProcess();
     qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
     connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),
	     this, SLOT(setValue()));

     proc->start("dig", host);
     
     exec();
     emit threadEnd(host, pout);
#ifndef DIG_NO_DEBUG
     qDebug() << "dig() THREAD:: Quit";
#endif
 }

void digThread::setValue() {
#ifndef DIG_NO_DEBUG
     qDebug() << "dig() THREAD:: -> start";
#endif
     pout  = proc->readAllStandardOutput(); // read std buffer
     delete proc;
     exit(0);
}

void digThread::stopProcess() {
#ifndef DIG_NO_DEBUG
     qDebug() << "dig() THREAD:: Stop Scan Process";
#endif
     if(proc) {
	  proc->terminate();
     }
}
