/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
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

void nmapClass::startProfile_ui()   // start preference UI
{
    mainProfile dialogPreference_(this);
    connect(&dialogPreference_, SIGNAL(accepted()),
            this, SLOT(readProfile()));

    dialogPreference_.exec();
}

void nmapClass::exit()
{
    _monitor->clearHostMonitor();
    freelist<lookupManager*>::itemDeleteAllWithWait(internealLookupList);
    freelist<digManager*>::itemDeleteAll(digLookupList);

    if (!FileName.isNull())
    {

        QFile *tmpFile = new QFile();
        QString newFile = logPath;
        newFile.append("/");
	newFile = QDir::toNativeSeparators(newFile);
        newFile.append(FileName);
        tmpFile->setFileName(newFile);

        if ((checkLog) && (tmpFile->exists()))
	{
            tmpFile->close();
            isEmptyLog();
        }
        else if ((!checkLog) && (tmpFile->exists()))
	{ // if log check is disable but the file exist
                tmpFile->close();
                tmpFile->remove();
        }

        delete tmpFile;
    }

    // Save window size and position and NSS info
    saveUiSettings();
    close();
}

void nmapClass::stop_scan()
{
    // stop and clear clear thread
    _monitor->clearHostMonitor();
    freelist<lookupManager*>::itemDeleteAllWithWait(internealLookupList);
    freelist<digManager*>::itemDeleteAll(digLookupList);
}
