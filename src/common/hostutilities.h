/*
Copyright 2011-2013  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HOSTUTILITIES_H
#define HOSTUTILITIES_H

#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QTreeWidgetItem>
#include <QtNetwork/QHostAddress>

#include "regularexpression.h"

class HostTools
{
public:
    static bool isDns(const QString& hostname) {
        /**
         * check if hostname is a dns.
         * QHostAddress is not null only if hostname is a ip.
         *
         **/

        // The check is valid for ipv4 and ipv6 address.
        QHostAddress hostAddress(hostname);

        if (!hostAddress.isNull()) {
            return false;
        }

        return true;
    }

    static bool isValidDns(const QString& hostname) {
        /**
         * Check for dns validity.
         *
         **/

        QRegExp dns(matchDNS);

        if (dns.indexIn(hostname) != -1) {
            return true;
        }

        return false;
    }

    static QString clearHost(const QString hostname) {
        // check for wrong dns address
        // (http:// ftp:// sftp:// https://)[dns/ip]
        QString hostNew_(hostname);
        if (hostname.startsWith(QLatin1String("http://"))) {
            hostNew_.remove("http://");
            return hostNew_;
        }

        if (hostname.startsWith(QLatin1String("https://"))) {
            hostNew_.remove("https://");
            return hostNew_;
        }

        if (hostname.startsWith(QLatin1String("ftp://"))) {
            hostNew_.remove("ftp://");
            return hostNew_;
        }

        if (hostname.startsWith(QLatin1String("sftp://"))) {
            hostNew_.remove("sftp://");
            return hostNew_;
        }

        return hostname;
    }

    static bool checkViewOS(const QString& OSline, QTreeWidgetItem *itemOS) {
        // check string OS for Icon selection
        if (OSline.contains("Linux")) {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/linux_logo.png")));
            return true;
        } else if (OSline.contains("Windows")) {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/windows_logo.png")));
            return true;
        } else if (OSline.contains("FreeBSD")) {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/freebsd_logo.png")));
            return true;
        } else if (OSline.contains("OpenBSD")) {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/openbsd_logo.png")));
            return true;
        } else if (OSline.contains("Solaris")) {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/solaris_logo.png")));
            return true;
        } else if (OSline.contains("Mac OS X")) {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/mac-os-x_logo.png")));
            return true;
        } /*else {
            itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/no-os.png")));
            return false;
        }*/

        return false;
    }

};

#endif // HOSTUTILITIES_H
