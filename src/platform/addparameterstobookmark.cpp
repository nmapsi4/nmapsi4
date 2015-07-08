/*
Copyright 2011-2015  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "addparameterstobookmark.h"

AddParametersToBookmark::AddParametersToBookmark(QWidget* parent, const QString parameters)
    : QDialog(parent)
{
    setupUi(this);

    connect(doneButt, &QPushButton::clicked,
            this, &AddParametersToBookmark::exit);
    connect(cancelButt, &QPushButton::clicked,
            this, &AddParametersToBookmark::close);
    lineProfilePar->setText(parameters);
}

AddParametersToBookmark::~AddParametersToBookmark()
{

}

void AddParametersToBookmark::exit()
{
    if (!lineProfileName->text().isEmpty() && !lineProfilePar->text().isEmpty()) {
        emit doneParBook(lineProfileName->text(), lineProfilePar->text());
        close();
    } else {
        QMessageBox::warning(this, tr("Warning - Nmapsi4"), tr("Insert profile name or/and profile parameters."), tr("Close"));
    }
}
