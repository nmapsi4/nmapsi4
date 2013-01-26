/*
Copyright 2008-2013  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QWidget>

#include "ui_about.h"
#include "config-nmapsi4.h"

class About : public QDialog, private Ui::AboutUi
{
public:
    explicit About(QWidget *parent = 0);
    ~About() {};

    static const QString description();
    static const QString copyright();
};

#endif
