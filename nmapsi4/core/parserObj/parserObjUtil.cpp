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

#include "parserObjUtil.h"

parserObjUtil::parserObjUtil() {

}

parserObjUtil::~parserObjUtil() {

}

QStringList parserObjUtil::getInfoLookup() {
    return mainLookup_;
}

QString parserObjUtil::getHostName() {
    return hostName_;
}

void parserObjUtil::setInfoLookup(const QString lookupElem) {
    mainLookup_.push_back(lookupElem);
}

void parserObjUtil::setHostName(const QString hostName) {
    hostName_.append(hostName);
}

