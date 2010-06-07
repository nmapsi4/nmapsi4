/***************************************************************************
 *   Copyright (C) 2007-2010 by Francesco Cecconi                          *
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

#include "../../mainwin.h"

void nmapClass::searchVuln()
{
    if (comboVulnRis->currentText().isEmpty())
        return;

    QString url;
    QString tmp;
    history = new logHistory("nmapsi4/cacheVuln", hostCache);
    history->addItemHistory(comboVulnRis->currentText());
    delete history;

    actSearch->setEnabled(false);
    actStop->setEnabled(true);
    actBack->setEnabled(true);
    actForward->setEnabled(true);
    
    tmp = comboVulnRis->currentText();
    tmp.replace(QString(" "), QString("+"));

    switch (comboWebV->currentIndex()) {
    case 0:
        url = "http://www.securityfocus.com/swsearch?sbm=bid&submit=Search%21&metaname=alldoc&sort=swishrank&query=";
        url.append(tmp);

#ifndef VULN_NO_DEBUG
        qDebug() << "Call webSearch()..." << url;
#endif
        break;
    case 1:
        url = "http://cve.mitre.org/cgi-bin/cvekey.cgi?keyword=";
        url.append(tmp);
#ifndef VULN_NO_DEBUG
        qDebug() << "Call webSearch()..." << url;
#endif
        break;
    case 2:
        url = "http://secunia.com/advisories/search/?search=";
        url.append(tmp);
#ifndef VULN_NO_DEBUG
        qDebug() << "Call webSearch()..." << url;
#endif
        break;
    }

    QUrl urlFinal(url);
    viewVuln->load(urlFinal);
}

void nmapClass::callSearchHistoryVuln() {
#ifndef VULN_NO_DEBUG
    qDebug() << "searchVuln:: call...";
#endif

    if (!actionAdd_Bookmark->isEnabled()) {
        actionAdd_Bookmark->setEnabled(true);
        action_Add_BookmarkToolBar->setEnabled(true);
    }
    logHistory *history = new logHistory("nmapsi4/cacheVuln", hostCache);
    history->searchHistory(comboVulnRis->currentText(), comboVulnRis);
    delete history;
     
}

void nmapClass::callVulnCheck() {
    comboVulnRis->clear();
    comboVulnRis->insertItem(0,treeBookVuln->currentItem()->text(0));
    tabVuln->setCurrentIndex(0);
    searchVuln();
}

void nmapClass::vulnPostScan() {
    actSearch->setEnabled(true);
    actStop->setEnabled(false);
}

void nmapClass::updateComboVuln(const QString& value) {

    comboVulnRis->clear();

    if(comboVuln->currentIndex()) {
        comboVulnRis->insertItem(0,value);
    } else {
        comboVulnRis->lineEdit()->clear();
    }

}
