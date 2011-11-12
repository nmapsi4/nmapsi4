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

#ifndef HOSTTOOLS_H
#define HOSTTOOLS_H

#include <QtCore/QString>
#include <QtCore/QRegExp>

class hostTools
{
public:
    static bool isDns(QString hostname)
    {
        QRegExp rx_("*.*.*.*");
        rx_.setPatternSyntax(QRegExp::Wildcard);
        return (rx_.exactMatch(hostname) == true) ? false : true;
    }
    
    static QString clearHost(const QString hostname)
    {
        // check for wrong dns address
        // (http:// ftp:// sftp:// https://)[dns/ip]
        QString hostNew_(hostname);
        if (hostname.startsWith(QLatin1String("http://")))
        {
            hostNew_.remove("http://");
            return hostNew_;
        }

        if (hostname.startsWith(QLatin1String("https://")))
        {
            hostNew_.remove("https://");
            return hostNew_;
        }

        if (hostname.startsWith(QLatin1String("ftp://")))
        {
            hostNew_.remove("ftp://");
            return hostNew_;
        }

        if (hostname.startsWith(QLatin1String("sftp://")))
        {
            hostNew_.remove("sftp://");
            return hostNew_;
        }

        return hostname;
    }
};

#endif // HOSTTOOLS_H
