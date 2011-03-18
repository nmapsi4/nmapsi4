/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
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

void nmapClass::closeVulnTab(int index) {
    if(!index) {
	//TODO [index = 0] load default page
	if (!webViewList[0]->url().toString().contains("about:blank")) {
	    // isn't empty, load default page
	    webViewList[0]->load(QUrl("about:blank"));
	    tWresult->setTabText(index, "");
	}
        return;
    }

    QWebView *tmp = webViewList.takeAt(index);
    qDebug() << "WebView pointer:: " << tmp << " Index:: " << index;
    tWresult->removeTab(index);
    delete tmp;
}

void nmapClass::tabWebBack() {
    qDebug() << "tab Back()";
    webViewList[tWresult->currentIndex()]->triggerPageAction(QWebPage::Back);
}

void nmapClass::tabWebForward() {
    qDebug() << "tab Forward()";
    webViewList[tWresult->currentIndex()]->triggerPageAction(QWebPage::Forward);
}

void nmapClass::tabWebStop() {
    qDebug() << "tab Stop()";
    webViewList[tWresult->currentIndex()]->triggerPageAction(QWebPage::Stop);
}
