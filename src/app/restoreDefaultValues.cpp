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

#include "mainwin.h"

void nmapClass::checkProfile()
{

    QSettings settings("nmapsi4", "nmapsi4");

    // TODO: load saved profile or simple default

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

#ifdef Q_WS_WIN
    // disable lookup in MS windows
    LookupEnabled = false;
#else
    LookupEnabled = settings.value("LookupEnabled", "false").toBool();
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

    // update max parallel scan option
    _monitor->updateMaxParallelScan();
}
