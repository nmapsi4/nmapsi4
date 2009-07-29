/***************************************************************************
 *   Copyright (C) 2008-2009 by Francesco Cecconi                          *
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
                       QStringList parametri, QObject *parent)
     : pout(ProcB1), 
       perr(ProcB2),
       ParList(parametri),
       par(parent)

{

}

void scanThread::run() {
     
     // move to constructor
     proc = new QProcess();
     qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
     connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),
	     this, SLOT(setValue()));

     // signal from parent
     connect(par, SIGNAL(killScan()),
	     this, SLOT(stopProcess())); 
     //scanMonitor->setText(1, tr("In progress..."));
     proc->start("nmap", ParList);
     
     exec();
     emit upgradePR();
     emit threadEnd(ParList[ParList.size()-1], pout, perr);
     qDebug() << "THREAD:: Quit";
 }

void scanThread::setValue() {
     qDebug() << "THREAD:: -> start";
     pout  = proc->readAllStandardOutput(); // read std buffer
     perr  = proc->readAllStandardError(); // read error buffer
     exit(0);
}

void scanThread::stopProcess() {
     qDebug() << "THREAD:: Stop Scan Process";
     if(proc) {
	  proc->terminate();
     }
}
