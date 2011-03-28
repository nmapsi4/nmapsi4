/***************************************************************************
 *   Copyright (C) 2007-2011 by Francesco Cecconi                          *
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

#include "../mainwin.h"


void nmapClass::about()
{
    mainAbout about;
    about.exec();
}

void nmapClass::about_qt()
{
    mainAbout about;
    about.qt();
}

void nmapClass::show_browser(QLineEdit *location)
{
    QString FileName;

    FileName = QFileDialog::getOpenFileName(this, QApplication::translate("nmapClass", "Select the file",
                                            0, QApplication::UnicodeUTF8), "/home", "");

    location->setText(FileName);
}


void nmapClass::show_log_browserUrl(const QString url, QLineEdit *location)
{
    QString FileName;

    FileName = QFileDialog::getExistingDirectory(this, "Open Directory",
               url, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(FileName.isEmpty()) {
        FileName.append(QDir::tempPath());
    }
    location->setText(FileName);
}

void nmapClass::input_browser()
{
    this->show_browser(lineInputFile);
}

void nmapClass::show_bugUrl() 
{
    QDesktopServices::openUrl(QUrl("https://launchpad.net/nmapsi4"));
}

void nmapClass::show_homepageUrl() 
{
    QDesktopServices::openUrl(QUrl("http://www.nmapsi4.org"));
}

void nmapClass::show_documentationUrl() 
{
    QDesktopServices::openUrl(QUrl(""));
}

void nmapClass::show_donateUrl() 
{
    QDesktopServices::openUrl(QUrl("http://www.nmapsi4.org"));
}
