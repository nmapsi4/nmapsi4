/***************************************************************************
 *   Copyright (C) 2010 by Francesco Cecconi                               *
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

#ifndef PARSEROBJ_H
#define PARSEROBJ_H

#include <QString>
#include <QStringList>

class parserObj
{
    public:
        parserObj();
        ~parserObj();

        // Getters
        QString getUptime();
        QString getTcpSequence();
        QString getDeviceType();
        QString getRunning();
        QStringList getServices();

        // Setters
        void setUptime(const QString uptime);
        void setTcpSequence(const QString tcpSequence);
        void setDeviceType(const QString deviceType);
        void setRunning(const QString Running);
        void setServices(const QString Service);

    protected:
        QString uptime_;
        QString tcp_sequence_;
        QString deviceType_;
        QString running_;
        QStringList services_;
};


#endif