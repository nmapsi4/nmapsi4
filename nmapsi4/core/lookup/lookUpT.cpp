/***************************************************************************
 *   Copyright (C) 2009-2011 by Francesco Cecconi                          *
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

#include "lookUpT.h"

lookUpT::lookUpT(const QString hostname, QObject *parent) : host(hostname), par(parent)
{
    connect(par, SIGNAL(killScan()),
            this, SLOT(killLookup()));

}

void lookUpT::run() {

    qRegisterMetaType<QHostInfo>("QHostInfo");
    info = QHostInfo::fromName(host);

    if(info.error() != QHostInfo::NoError && info.error() != QHostInfo::UnknownError) {
#ifdef LOOKUP_NO_THREAD
         qDebug() << "Lookup failed:" << info.errorString();
#endif
         emit threadEnd(info, -1, host);
         return;
     }
    emit threadEnd(info, 1, host);
}

void lookUpT::killLookup() {
#ifdef LOOKUP_NO_THREAD
    qDebug() << "Lookup::kill --> call";
#endif
    info.abortHostLookup(info.lookupId());
}
