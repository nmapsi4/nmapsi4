/***************************************************************************
 *   Copyright (C) 2008-2011 by Francesco Cecconi                          *
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

#include "loghistory.h"

//
// @Developer: Francesco Cecconi
//
void logHistory::coreItemHistory(const QString url, const QString scanTime)
{
    Q_ASSERT(!url.isEmpty());

    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    QList<QString> urlList;
    QList<QString> urlListTime;

    urlList = historyReadUrl();
    if (!scanTime.isNull())
    {
        urlListTime = historyReadUrlTime();
    }

    if (urlList.contains("NULL"))
    {
        urlList.removeFirst();
        urlList.append(url);
        settings.setValue(configTag, QVariant(urlList));
        if (!scanTime.isNull())
        {
            urlListTime.removeFirst();
            urlListTime.append(scanTime);
            settings.setValue(configTagTime, QVariant(urlListTime));
        }
    }
    else if (((urlList.size() == m_cacheSize)
              && (m_cacheSize != -1))
             && (!urlList.contains(url)))
    {
        urlList.removeLast();
        urlList.push_front(url);
        settings.setValue(configTag, QVariant(urlList));

        if (!scanTime.isNull())
        {
            urlListTime.removeLast();
            urlListTime.push_front(scanTime);
            settings.setValue(configTagTime, QVariant(urlListTime));
        }
    }
    else if (!urlList.contains(url))
    {
        urlList.push_front(url);
        settings.setValue(configTag, QVariant(urlList));

        if (!scanTime.isNull())
        {
            urlListTime.push_front(scanTime);
            settings.setValue(configTagTime, QVariant(urlListTime));
        }
    }
    else if (!scanTime.isNull())
    {
        int index = urlList.indexOf(url);
        urlListTime[index].clear();
        urlListTime[index].append(QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));
        settings.setValue(configTagTime, QVariant(urlListTime));
    }
}

