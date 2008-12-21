#include "scanThread.h" 

scanThread::scanThread(QByteArray* ProcB1, QByteArray* ProcB2, 
		       QStringList parametri, QObject *parent)
     : pout(ProcB1), 
       perr(ProcB2),
       ParList(parametri),
       par(parent)
{

}

void scanThread::run() {
     
     proc = new QProcess();
     qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
     connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),
	     this, SLOT(setValue())); // nmapParser.cpp

     // signal from parent
     connect(par, SIGNAL(killScan()),
	     this, SLOT(stopProcess())); 

     proc->start("nmap", ParList);
     
     exec();
     emit upgradePR();
     emit threadEnd(ParList[ParList.size()-1]);
     qDebug() << "THREAD:: Quit";
 }

void scanThread::setValue() {
     
     qDebug() << "THREAD:: -> start";
     *pout  = proc->readAllStandardOutput(); // read std buffer
     *perr  = proc->readAllStandardError(); // read error buffer
     exit(0);
}

void scanThread::stopProcess() {
     qDebug() << "THREAD:: Stop Scan Process";
     if(proc) {
	  proc->terminate();
     }
}
