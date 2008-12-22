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

#include "../mainwin.h"

void nmapClass::readProfile()
{

    this->checkProfile();

    if (checkLog)
        this->fileSession();

}

void nmapClass::resetOptions()
{
    versionBox->setChecked(false);
    checkOS->setChecked(false);
    comboTiming->setCurrentIndex(0);
    comboDNSResolv->setCurrentIndex(0);
    comboVerbosity->setCurrentIndex(0);
}

void nmapClass::checkProfile()
{

    int uid = 0;

#ifndef Q_WS_WIN
    uid = getuid();
#endif

    QSettings settings("nmapsi4", "nmapsi4");

    Profile = settings.value("configProfile", "none").toString();

    if (!Profile.compare("none")) {
        settings.setValue("configProfile", "normal"); // default value
        Profile = settings.value("configProfile", "none").toString();
    }
    
    if ((Profile.contains("fullversion") || Profile.contains("quickversion")) && uid) {
	 Profile = "normal"; // reset profile
	 settings.setValue("configProfile", Profile);
	 lineProfile->setText(Profile);
    } else if (!Profile.contains("fullversion") && !Profile.contains("quickversion") && !uid) {
	 Profile = "fullversion"; // reset profile
	 settings.setValue("configProfile", Profile);
	 lineProfile->setText(Profile);
    }

    
    if (Profile.contains("quick"))
	 this->setQuickProfile();
    else if (Profile.contains("normal"))
	 this->setNormalProfile();
    else if (Profile.contains("fullversion"))
        this->setFullVersionProfile();
    else if (Profile.contains("quickversion"))
	 this->setQuickVersionProfile();
    
    
    logPath = settings.value("confPath", "none").toString();
    if (!logPath.compare("none")) {
        QString path_string = QDir::homePath();
        path_string.append("/");
        settings.setValue("confPath", path_string); // default value
        logPath = path_string;
    }
    
    QString log = settings.value("logCheck", "none").toString();
    
    if ((!log.compare("none")) || (log.compare("true") && (log.compare("false")))) {
	 settings.setValue("logCheck", "false"); // default value
        log = "false";
    }
    
    if (!log.compare("true")) {
	 checkLog = true;
	 action_Save_As->setEnabled(false);
	 actionSave->setEnabled(false);
    } else
	 checkLog = false;
    
    QString Vlog = settings.value("Vlog", "none").toString();

    if ((!Vlog.compare("none"))) {
	 settings.setValue("Vlog", "false"); // default value
    }
    
    verboseLog = settings.value("Vlog").toBool();
    qDebug() << "nmapsi4/nmapParser --> verboseLog::" << verboseLog;
    
    QString pos = settings.value("savePos", "none").toString();

    if ((!pos.compare("none")) || (pos.compare("true") && (pos.compare("false")))) {
        settings.setValue("savePos", "true"); // default value
        pos = "true";
    }
    
    if (!pos.compare("true"))
	 savePos = true;
    else
	 savePos = false;

    QString size = settings.value("saveSize", "none").toString();
    
    if ((!size.compare("none")) || (size.compare("true") && (size.compare("false")))) {
        settings.setValue("saveSize", "true"); // default value
        size = "true";
    }
    
    if (!size.compare("true"))
        saveSize = true;
    else
	 saveSize = false;

    QString vulnExt = settings.value("vulnFlag", "none").toString();
    if((!vulnExt.compare("none"))) {
	 settings.setValue("vulnFlag", "true"); // default value
    } else if((vulnExt.compare("false"))) {
	 comboMain->insertItem(2, tr("Vulnerabilities"));
    } else {
	 if(comboMain->currentIndex() == 2) {
	      stackedMain->setCurrentIndex(0);
	      comboMain->setCurrentIndex(0);
	 }
	 
	 comboMain->removeItem(2);
    }
}

void nmapClass::setQuickProfile()
{
    this->resetOptions();
    comboTiming->setCurrentIndex(4);
    comboDNSResolv->setCurrentIndex(2);
    comboVerbosity->setCurrentIndex(1);
}

void nmapClass::setNormalProfile()
{
    this->resetOptions();
    comboTiming->setCurrentIndex(4);
    comboDNSResolv->setCurrentIndex(2);
    comboVerbosity->setCurrentIndex(1);
    versionBox->setChecked(true);
}

void nmapClass::setFullVersionProfile()
{
    this->resetOptions();
    comboTiming->setCurrentIndex(4);
    checkOS->setChecked(true);
    versionBox->setChecked(true);
    comboVerbosity->setCurrentIndex(1);
    comboDNSResolv->setCurrentIndex(2);
}

void nmapClass::setQuickVersionProfile()
{
    this->resetOptions();
    comboTiming->setCurrentIndex(4);
    checkOS->setChecked(true);
    comboDNSResolv->setCurrentIndex(2);
}
