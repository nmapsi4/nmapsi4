/***************************************************************************
 *   Copyright (C) 2007-2008 by Francesco Cecconi                          *
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

void nmapClass::searchVuln() {
     // TODO
     if(comboVuln->currentText().isEmpty())
	  return;

     QString url ="http://www.securityfocus.com/swsearch?query=";
     QString tmp = comboVuln->currentText();
     tmp.replace(QString(" "), QString("+"));
     url.append(tmp);
     url.append("&sbm=bid&submit=Search%21&metaname=alldoc&sort=swishrank");
     qDebug() << "Call webSearch()..." << url;
     QUrl urlFinal(url);
     viewVuln->load(urlFinal);
}
