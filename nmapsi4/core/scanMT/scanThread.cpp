/***************************************************************************
 *   Copyright (C) 2008-2010 by Francesco Cecconi                          *
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

#include "scanThread.h" 

scanThread::scanThread(QByteArray& ProcB1, QByteArray& ProcB2,
                       const QStringList parametri, QObject *parent)
     : pout(ProcB1), 
       perr(ProcB2),
       ParList(parametri),
       par(parent)
{

    connect(par, SIGNAL(killScan()),
            this, SLOT(stopProcess()));

}

void scanThread::run() {
     
     proc = new QProcess();
     qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

     connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),
             this, SLOT(setValue()));

     qDebug() << "DEBUG::ThreadString:: " << ParList;
     proc->start("nmap", ParList);
     
     exec();
     emit upgradePR();
     emit threadEnd(ParList[ParList.size()-1], pout, perr);
     qDebug() << "scan() THREAD:: Quit";
 }

void scanThread::setValue() {
     qDebug() << "scan() THREAD:: -> start";
     pout  = proc->readAllStandardOutput(); // read std buffer
     perr  = proc->readAllStandardError(); // read error buffer
     //delete proc;
     exit(0);
}

void scanThread::stopProcess() {
     qDebug() << "scan() THREAD:: Stop Scan Process";
     if(proc) {
	  proc->terminate();
     }
}
