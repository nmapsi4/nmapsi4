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

#include "about.h"

About::About(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    textDescription->setText(description() + "<br/><br/>" + copyright());
    textAuthors->setText(authors());
    textSupport->setText(support());
    textThanks->setText(thanksTo());
    labelVersion->setText(labelVersion->text() + VERSION);
}

const QString About::description()
{
    return QApplication::tr("NmapSi4 is a complete Qt4-based Gui with "
                            "the design goals to provide a complete nmap "
                            "interface for users, in order to manage al "
                            "option of this power security net scanner "
                            "and search services vulnerability.");
}


const QString About::copyright()
{
    return QApplication::tr("(c) 2007-2013 Francesco Cecconi"
                            "<br/>License: GNU General Public License Version 2");

}

const QString About::authors()
{
    QString authorsList;

    authorsList.append("<strong>Francesco Cecconi</strong><br/>");
    authorsList.append(QApplication::tr("Maintainer and developer") + "<br/>");
    authorsList.append("<a href=\"mailto:brand@nmapsi4.org\">brand@nmapsi4.org</a>");

    return authorsList;
}

const QString About::support()
{
    QString supportList;

    supportList.append(QApplication::tr("HomePage") + "<br/>");
    supportList.append("<a href=\"http://www.nmapsi4.org\">www.nmapsi4.org</a>");
    supportList.append("<br/><br/>");
    supportList.append(QApplication::tr("Bug Tracking") + "<br/>");
    supportList.append("<a href=\"http://bugs.nmapsi4.org\">bugs.nmapsi4.org</a>");
    supportList.append("<br/><br/>");
    supportList.append(QApplication::tr("Sources Repository") + "<br/>");
    supportList.append("<a href=\"https://github.com/nmapsi4/nmapsi4\">github.com/nmapsi4/nmapsi4</a>");
    supportList.append("<br/><br/>");
    supportList.append(QApplication::tr("Forum") + "<br/>");
    supportList.append("<a href=\"http://forum.nmapsi4.org\">forum.nmapsi4.org</a>");

    return supportList;
}

const QString About::thanksTo()
{
    QString thanksToList;

    thanksToList.append("<strong>Innocenzo Ventre</strong><br/>");
    thanksToList.append(QApplication::tr("MS Windows Packages and Italian localization") + "<br/>");
    thanksToList.append("<a href=\"mailto:el.diabl09@gmail.com\">el.diabl09@gmail.com</a>");
    thanksToList.append("<br/><br/>");
    thanksToList.append("<strong>Pavel Fric</strong><br/>");
    thanksToList.append(QApplication::tr("Czech localization") + "<br/>");
    thanksToList.append("<a href=\"mailto:pavelfric@seznam.cz\">pavelfric@seznam.cz</a>");
    thanksToList.append("<br/><br/>");
    thanksToList.append("<strong>Vincenzo Reale</strong><br/>");
    thanksToList.append(QApplication::tr("Italian localization") + "<br/>");
    thanksToList.append("<a href=\"mailto:smart2128@baslug.org\">smart2128@baslug.org</a>");

    return thanksToList;
}
