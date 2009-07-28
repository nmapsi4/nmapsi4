/***************************************************************************
 *   Copyright (C) 2007-2009 by Francesco Cecconi                          *
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

#include "profilemain.h"

void mainProfile::checkExtension() {
    //TODO
    QSettings extension("nmapsi4", "nmapsi4");
    
    vulnExt = extension.value("vulnFlag", "false").toBool();
    extList->clear(); // clear listWidget for reload use
    
    extList->setIconSize(QSize::QSize(48, 48));
    QIcon imgA = QIcon(QString::fromUtf8(":/images/nmapsi4/preference/images/preferences-pluginA.png"));
    QIcon imgD = QIcon(QString::fromUtf8(":/images/nmapsi4/preference/images/preferences-pluginD.png"));
    
    extPItem = new QListWidgetItem(extList);
    if(vulnExt) {
      extPItem->setIcon(imgA);
    } else {
      extPItem->setIcon(imgD);
    }
    extPItem->setText(tr("Check Vulnerabilities browser extension"));
}

void mainProfile::activeExt() {
    QSettings extension("nmapsi4", "nmapsi4");
    switch(extList->currentRow()) {
      case 0:
	if(!vulnExt) {
	    extension.setValue("vulnFlag", "true");
	    checkExtension();
	}
	break;
    }
}

void mainProfile::disableExt() {
    QSettings extension("nmapsi4", "nmapsi4");
    switch(extList->currentRow()) {
      case 0:
	if(vulnExt) {
	    extension.setValue("vulnFlag", "false");
	    checkExtension();
	}
	break;
    }
}
