/*
Copyright 2012-2013  Francesco Cecconi <francesco.cecconi@gmail.com>

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


#ifndef PACKAGE_H
#define PACKAGE_H

#include "config-nmapsi4.h"

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QUrl>

class Package
{
public:
    static QString localePath();
    static QUrl qmlPath(QString qmlScript);
};

#endif // PACKAGE_H
