/***************************************************************************
 *   Copyright (C) 2007-2011 by Francesco Cecconi                          *
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

#include "../../mainwin.h"

void nmapClass::readProfile()
{
    this->checkProfile();

    if (checkLog) 
    {
        this->fileSession();
    }
}

void nmapClass::checkProfile()
{

    QSettings settings("nmapsi4", "nmapsi4");
    
    // saved global profile
    globalProfile = settings.value("globalProfile", false).toBool();
    if (globalProfile) 
    {
        // call restore global profile
        qDebug() << "DEBUG:: Set global profile";
        restoreGlobalProfile();
    } 
    else 
    {
        qDebug() << "DEBUG:: Set local profile";
        settings.setValue("globalProfile", false); // default value
        removeGlobalButton->setEnabled(false);

        Profile = settings.value("configProfile", "none").toString();

        if (!Profile.compare("none")) 
        {
            settings.setValue("configProfile", "normal"); // default value
            Profile = settings.value("configProfile", "none").toString();
        }

        if ((Profile.contains("fullversion") || Profile.contains("quickversion")) && uid) 
        {
            Profile = "normal"; // reset profile
            settings.setValue("configProfile", Profile);
        } 
        else if (!Profile.contains("fullversion") && !Profile.contains("quickversion") && !uid) 
        {
            Profile = "fullversion"; // reset profile
            settings.setValue("configProfile", Profile);
        }

        if (Profile.contains("quick")) 
        {
            this->setQuickProfile();
        } 
        else if (Profile.contains("normal")) 
        {
            this->setNormalProfile();
        } 
        else if (Profile.contains("fullversion")) 
        {
            this->setFullVersionProfile();
        } 
        else if (Profile.contains("quickversion")) 
        {
            this->setQuickVersionProfile();
        }
    }
    
    logPath = settings.value("confPath", "none").toString();
    if (!logPath.compare("none")) 
    {
        QString path_string = QDir::homePath();
        path_string.append("/");
        settings.setValue("confPath", path_string); // default value
        logPath = path_string;
    }
    
    QString log = settings.value("logCheck", "none").toString();
    
    if ((!log.compare("none")) || (log.compare("true") && (log.compare("false")))) 
    {
        settings.setValue("logCheck", "false"); // default value
        log = "false";
    }
    
    if (!log.compare("true")) 
    {
        checkLog = true;
        action_Save_As->setEnabled(false);
        actionSave->setEnabled(false);
    } 
    else 
    {
        checkLog = false;
    }
    
    QString Vlog = settings.value("Vlog", "none").toString();

    if ((!Vlog.compare("none"))) 
    {
        settings.setValue("Vlog", "false"); // default value
    }
    
    verboseLog = settings.value("Vlog").toBool();
    
    QString pos = settings.value("savePos", "none").toString();

    if ((!pos.compare("none")) || (pos.compare("true") && (pos.compare("false")))) 
    {
        settings.setValue("savePos", "true"); // default value
        pos = "true";
    }
    
    if (!pos.compare("true")) 
    {
        savePos = true;
    } 
    else 
    {
        savePos = false;
    }

    QString size = settings.value("saveSize", "none").toString();
    
    if ((!size.compare("none")) || (size.compare("true") && (size.compare("false")))) 
    {
        settings.setValue("saveSize", "true"); // default value
        size = "true";
    }
    
    if (!size.compare("true")) 
    {
        saveSize = true;
    } 
    else 
    {
        saveSize = false;
    }


    QString cache = settings.value("hostCache", "none").toString();

    if (!cache.compare("none")) 
    {
        settings.setValue("hostCache", "10"); // default value
    }

    hostCache = settings.value("hostCache").toInt();

    QString lookInt = settings.value("lookInternal", "none").toString();

    if ((!lookInt.compare("none"))) 
    {
         settings.setValue("lookInternal", "true"); // default value
    }

    lookupInternal = settings.value("lookInternal").toBool();

#ifdef Q_WS_WIN
    // disable lookup in MS windows
    lookupInternal = false;
#endif

    QString lDig = settings.value("lookDig", "none").toString();

    if ((!lDig.compare("none"))) 
    {
         settings.setValue("lookDig", "false"); // default value
    }

    lookupDig = settings.value("lookDig").toBool();

#ifdef Q_WS_WIN
    // disable lookup in MS windows
    lookupDig = false;
#endif

    QString nss_ = settings.value("NSSsupport", "none").toString();

    if ((!nss_.compare("none"))) 
    {
         settings.setValue("NSSsupport", "true"); // default value
    }

    NSSsupport = settings.value("NSSsupport").toBool();

    QString adv_ = settings.value("ADVSupport", "none").toString();

    if ((!adv_.compare("none"))) 
    {
         settings.setValue("ADVSupport", "true"); // default value
    }

    ADVSupport = settings.value("ADVSupport").toBool();

    QString MonitorEnabled_ = settings.value("MonitorEnabled", "none").toString();

    if ((!MonitorEnabled_.compare("none"))) 
    {
         settings.setValue("MonitorEnabled", "true"); // default value
    }

    MonitorEnabled = settings.value("MonitorEnabled").toBool();

    QString LookupEnabled_ = settings.value("LookupEnabled", "none").toString();

    if ((!LookupEnabled_.compare("none"))) 
    {
         settings.setValue("LookupEnabled", "true"); // default value
    }

#ifdef Q_WS_WIN
    // disable lookup in MS windows
    LookupEnabled = false;
#else
    LookupEnabled = settings.value("LookupEnabled").toBool();
#endif

    QString TracerouteEnabled_ = settings.value("TraceEnabled", "none").toString();

    if ((!TracerouteEnabled_.compare("none"))) 
    {
         settings.setValue("TraceEnabled", "true"); // default value
    }

    TraceEnabled = settings.value("TraceEnabled").toBool();
    
    // restore actionMenuBar
    
    bool actionMB = settings.value("showMenuBar", false).toBool();
    
    if (actionMB) 
    {
        actionMenuBar->setChecked(true);
    } 
    else 
    {
        actionMenuBar->setChecked(false);
    }
    
    // restore nse Combo script
    int nseComboScriptTmp_ = settings.value("nseComboScript", 0).toInt();
    updateNseOptionScript(nseComboScriptTmp_);
    
    nseScriptActiveList = settings.value("nseScriptActiveList","none").toStringList();
    nseScriptAvailList = settings.value("nseScriptAvailList","none").toStringList();
    
    if (!nseScriptAvailList.first().compare("none")) 
    {
        nseTreeDefaultValue();
    } 
    else 
    {
        if (!nseScriptAvailList.first().compare("")) 
        {
            nseScriptAvailList.removeFirst();
        }

        if (!nseScriptActiveList.first().compare("")) 
        {
            nseScriptActiveList.removeFirst();
        }
        nseTreeAvailRestoreValues();
        nseTreeActiveRestoreValues();
    }
}
