/***************************************************************************
 *   Copyright (C) 2008-2010 by Francesco Cecconi                          *
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

#include "about.h"

mainAbout::mainAbout() {
    setupUi(this);
    QString version = labelVersion->text();
    QString versionV = labelVuln->text();
    QString versionDig_ = labelDig->text();

    version.append(_VERSION_);
    versionV.append(_VERSION_VULN_);
    versionDig_.append(_VERSION_DIG_);
    labelVersion->setText(version);
    labelVuln->setText(versionV);
    labelDig->setText(versionDig_);
}

void mainAbout::qt() {
    QMessageBox::aboutQt(this, "Qt Version");
}

mainAbout::~mainAbout() {
}
