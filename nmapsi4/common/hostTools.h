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
#include <QtGui/QTreeWidgetItem>

class hostTools
{
public:
    static bool isDns(const QString& hostname)
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
    
    static bool checkViewOS(const QString& OSline, QTreeWidgetItem *itemOS)
    {
        // check string OS for Icon selection
        itemOS->setTextAlignment(1, Qt::AlignVCenter | Qt::AlignRight);

        if (OSline.contains("Linux"))
        {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/linux_logo.png")));
            itemOS->setText(1, "GNU/Linux");
            return true;
        }
        else if (OSline.contains("Windows"))
        {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/windows_logo.png")));
            itemOS->setText(1, "MS Windows");
            return true;
        }
        else if (OSline.contains("FreeBSD"))
        {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/freebsd_logo.png")));
            itemOS->setText(1, "FreeBSD");
            return true;
        }
        else if (OSline.contains("OpenBSD"))
        {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/openbsd_logo.png")));
            itemOS->setText(1, "OpenBSD");
            return true;
        }
        else if (OSline.contains("Solaris"))
        {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/solaris_logo.png")));
            itemOS->setText(1, "Solaris");
            return true;
        }
        else if (OSline.contains("Mac OS X"))
        {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/mac-os-x_logo.png")));
            itemOS->setText(1, "MacOsX");
            return true;
        }
        else
        {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/no-os.png")));
            itemOS->setText(1, "Undiscovered");
            return false;
        }
    }

};

#endif // HOSTTOOLS_H
