/*
Copyright 2008-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "about.h"

About::About(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    descriptionLabel->setText(description());
    labelVersion->setText(labelVersion->text() + VERSION);
}

QString About::description()
{
    return QApplication::tr("NmapSi4 is a complete Qt4-based Gui with <br/>"
                            "the design goals to provide a complete nmap<br/>"
                            "interface for users, in order to manage all<br/>"
                            "option of this power security net scanner<br/>"
                            "and search services vulnerability.");
}
