/*
    Copyright (C) 2011  Francesco Cecconi <francesco.cecconi@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef MAINDISCOVER_H
#define MAINDISCOVER_H

#include <QList>
#include <QNetworkInterface>

class mainDiscover
{

public:
    mainDiscover();
    ~mainDiscover();
    QList<QNetworkInterface> getAllInterfaces() const;
    QList<QNetworkAddressEntry> getAddressEntries(QNetworkInterface interface) const;
};

#endif // MAINDISCOVER_H
