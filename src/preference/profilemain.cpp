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

#include "profilemain.h"

mainProfile::mainProfile()
{
     QFont serifFont("Sans", 9);
     QApplication::setFont (serifFont);

     setupUi(this);
     QSettings ptrFile("nmapsi4","nmapsi4");
	
     connect( buttonSave, SIGNAL( clicked(bool) ), 
	      this, SLOT( quit()) );
     connect( checkNormalScan, SIGNAL( toggled(bool) ), 
	      this, SLOT(updateNormalCheck()));
     connect( checkQuickScan, SIGNAL( toggled(bool) ), 
	      this, SLOT(updateQuickCheck()));
     connect( checkFullVersion, SIGNAL( toggled(bool) ), 
	      this, SLOT(updateFullVersionCheck()));
     connect( checkQuickVersion, SIGNAL( toggled(bool) ), 
		 this, SLOT(updateQuickVersionCheck()));
     
     this->setProfile(); // set profile preference
     
	// create a read log config
     QString path = ptrFile.value("confPath", "none").toString();
     QString logCheck = ptrFile.value("logCheck", "none").toString();
     lineEditPath->setText(path);

     if(logCheck.contains("false")) {
	  checkLogOn->setChecked(false);
	  lineEditPath->setDisabled(true);
	  buttonLogB->setDisabled(true);
     }
     else
	  checkLogOn->setChecked(true);
     
     QString tmpSavePos = ptrFile.value("savePos", "none").toString();
     if(tmpSavePos.contains("true"))
	  checkWinPos->setChecked(true);
     else
	  checkWinPos->setChecked(false);
     
     QString tmpSize = ptrFile.value("saveSize", "none").toString();
     if(tmpSize.contains("true"))
	  checkSize->setChecked(true);
     else
	  checkSize->setChecked(false);

     bool Vlog = ptrFile.value("Vlog").toBool();
     if(Vlog)
	  checkVerboseLog->setChecked(true);
     else
	  checkVerboseLog->setChecked(false);

     // Create listview items
     listViewOptions->setIconSize(QSize::QSize (52, 52));

     profileItem = new QListWidgetItem(listViewOptions);
     profileItem->setIcon(QIcon(QString::fromUtf8(":/images/images/preferences-system-windows.png")));
     profileItem->setText(tr("Profiles"));
     
     logItem = new QListWidgetItem(listViewOptions);
     logItem->setIcon(QIcon(QString::fromUtf8(":/images/images/utilities-log-viewer.png")));
     logItem->setText(tr("Log"));
     
     sizeItem = new QListWidgetItem(listViewOptions);
     sizeItem->setIcon(QIcon(QString::fromUtf8(":/images/images/view-fullscreen.png")));
     sizeItem->setText(tr("Size"));
	
     profileItem->setSelected(true);
     labelTitle->setText(tr("<h3>Profiles Scan</h3>"));

     connect( checkLogOn, SIGNAL(pressed()), 
	      this, SLOT(update_saveButton()));
     connect(listViewOptions, SIGNAL(itemSelectionChanged()), 
	     this, SLOT(updateItem()));
     connect(buttonLogB, SIGNAL(clicked()), 
	     this, SLOT(log_browser())); 
     connect(buttonDefault, SIGNAL(clicked()), 
	     this, SLOT(setDefaults())); 

}

void mainProfile::saveProfile(QString ProfileType) {
     QSettings ptrFile("nmapsi4","nmapsi4");
     ptrFile.setValue("configProfile", ProfileType); // default value
}

QString mainProfile::readProfile() {
     QSettings ptrFile("nmapsi4","nmapsi4");
     QString tmpProfile = ptrFile.value("configProfile", "none").toString();
     return tmpProfile;
}

void mainProfile::setScan() {
     QSettings ptrFile("nmapsi4","nmapsi4");
     ptrFile.setValue("configProfile", ScanActive); // default value
     ptrFile.setValue("confPath", lineEditPath->text()); // default value

     if(checkLogOn->isChecked()) // update checklog from configuration file
	  ptrFile.setValue("logCheck", "true"); 
     else
	  ptrFile.setValue("logCheck", "false"); 

     if(checkWinPos->isChecked()) 
	  ptrFile.setValue("savePos", "true"); 
     else
	  ptrFile.setValue("savePos", "false"); 
     
      if(checkSize->isChecked()) 
	   ptrFile.setValue("saveSize", "true"); 
      else
	   ptrFile.setValue("saveSize", "false"); 

     if(checkVerboseLog->isChecked()) 
	   ptrFile.setValue("Vlog", "true"); 
      else
	   ptrFile.setValue("Vlog", "false"); 
}


void mainProfile::updateItem() {
     QString itemText = listViewOptions->currentItem()->text();

     if(itemText.contains("Profiles")) {
	  labelTitle->setText("<h3>Profiles Scan</h3>");
	  stackPref->setCurrentIndex(0);
     }
     
     if(itemText.contains("Log")) {
	  labelTitle->setText("<h3>Automatic Logs Options</h3>");
	      stackPref->setCurrentIndex(1);
     }
     
     if(itemText.contains("Size")) {
	  labelTitle->setText("<h3>Size Options</h3>");
	  stackPref->setCurrentIndex(2);
	}
}

void mainProfile::log_browser()
{
     QString FileName;
     QString url = QDir::homePath();
     
     FileName = QFileDialog::getExistingDirectory(this, "Open Directory",
						  url, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
     
     lineEditPath->setText(FileName);
}

void mainProfile::setProfile() {
     
     int uid = 0;
     
#ifndef Q_WS_WIN
     uid = getuid();
#endif
	
     QString tmpProfile = readProfile();
     
     if(uid) 
	  groupRoot->setVisible(false);
     else {
	  checkNormalScan->setEnabled(false);
	  checkQuickScan->setEnabled(false);
     }
	
     if(!tmpProfile.compare("normal")) {
	  checkNormalScan->setChecked(true);
	  ScanActive = "normal";
     }
	
     if(!tmpProfile.compare("quick")) {
	  checkQuickScan->setChecked(true);
	  ScanActive = "quick";
     }
	
	
     if(!tmpProfile.compare("fullversion") && !uid) {
	  checkFullVersion->setChecked(true);
	  ScanActive = "fullversion";
     } else if(uid)
	  checkFullVersion->setEnabled(false);
	
     if(!tmpProfile.compare("quickversion") && !uid) {
	  checkQuickVersion->setChecked(true);
	  ScanActive = "quickversion";
     } else if(uid)
	  checkQuickVersion->setEnabled(false);
	
}

void mainProfile::quit() {
     this->setScan(); // save Options
     emit accept();   // send accept signal and exit
}

void mainProfile::setDefaults() {
     checkNormalScan->setChecked(true);
     checkQuickScan->setChecked(false);
     checkFullVersion->setChecked(false);
     checkQuickVersion->setChecked(false);
     checkLogOn->setChecked(false);
     checkSize->setChecked(false);
     checkWinPos->setChecked(false);
     checkVerboseLog->setChecked(false);
}

mainProfile::~mainProfile() {
     delete profileItem;
     delete logItem;
     delete sizeItem;
}
