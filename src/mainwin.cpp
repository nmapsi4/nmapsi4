/***************************************************************************
 *   Copyright (C) 2007-2008 by Francesco Cecconi                          *
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

#include "mainwin.h"

nmapClass::nmapClass() : PFile(0), 
			 labelVersion(0), 
			 dialog(0), 
			 proc(0)
{
     int uid = 0;
	
#ifndef Q_WS_WIN
     uid = getuid();
#endif

     init();
     this->setNmapsiSlot();
	
     // Init function
     this->rootMode(uid); // send uid value
     this->checkProfile();
}

void nmapClass::init() {

     //QApplication::setStyle(new QPlastiqueStyle);
     setupUi(this);
     progressScan = new QProgressBar();
     progressScan->setValue(0);
     progressScan->setLayoutDirection(Qt::LeftToRight);
     statusBar()->addPermanentWidget(progressScan,2);
     checkNmapVersion();
     QSettings settings("nmapsi4","nmapsi4");
     QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
     QSize size = settings.value("window/size", QSize(869, 605)).toSize();
     resize(size);
     move(pos);
}

void nmapClass::scan()
{
     if(hostEdit->text().isEmpty() && lineInputFile->text().isEmpty()) {
	  QMessageBox::warning(this, "NmapSI4",tr("No Host Target\n"),tr("Close"));
	     return;
     }
     
     if(checkLog) { // create a file log
	  this->fileSession();
     } else {
	  if(!logPath.contains(QDir::tempPath())) {
	       QSettings ptr("nmapsi4","nmapsi4");
	       ptr.setValue("confPath",QDir::tempPath()); 
	       logPath = QDir::tempPath();
	       this->checkProfile();
	  }
	  
	  this->fileSession();
     }
     
     
     QStringList parametri; //parameters list declaration
     QString hostname = hostEdit->text();
     QString *title = new QString;
     *title = "NmapSI4 ";
	
     action_Scan_menu->setEnabled(false);
     action_Scan_2->setEnabled(false);
     hostEdit->setEnabled(false);
     actionStop_Scan->setEnabled(true);
     action_Save_As->setEnabled(false);
     actionSave_As_Menu->setEnabled(false);
     actionSave->setEnabled(false);
     actionSave_Menu->setEnabled(false);
	
     proc = new QProcess(); // Scan Process declaration
     connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), 
	     this, SLOT(nmapParser())); // nmapParser.cpp
     
     parametri = this->check_extensions(parametri,title); // extensions.cpp

     QString tmp_token;
     foreach(QString token, parametri) { // print scan options
	  tmp_token.append(token);
	  tmp_token.append(" "); // add simple space in option string
     }

     lineOptions->setText(tmp_token);
     
     parametri << hostname; // parameters list
     proc->start("nmap",parametri);
     
     if(proc->exitStatus() == 0)
	  qDebug() << "Nmapsi4/core -> ProcExitCode::" << proc->exitCode();
     else {
	  qFatal( "Nmapsi4/core -> Error with nmap process\n");
	  this->stop_scan();
     }

     this->setWindowIcon(QIcon(QString::fromUtf8(":/images/icons/nmapsi4_scan.svg")));
     progressScan->setValue(60);
     this->setWindowTitle(title->replace("(55%)","(60%)"));

     delete title;

}

nmapClass::~nmapClass()
{
     qDebug() << "Nmapsi4/~nmapClass() -> Global";
     qDebug() << "Nmapsi4/~nmapClass() -> Size Item List::" << itemList.size();
     if(dialog) dialog->close();


     if(!listClearFlag) {
	  itemDeleteAll(itemList);
     }		 
     delete progressScan;
     delete PFile;
     delete labelVersion;
     delete userMode;
}
