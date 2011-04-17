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

void nmapClass::readFlowFromThread(const QString hostname, const QString lineData) 
{
    /*
     * read data line form thread
     */
    QHash<QString, QStringList>::const_iterator i = scanHashListFlow.find(hostname);
    
    if (i == scanHashListFlow.end()) {
	QStringList flowHistory;
	flowHistory.append(lineData);
	foreach (const QString &token, flowHistory) {
	    qDebug() << "DEBUG:: scan CREATE:: " << token;
	}
	scanHashListFlow.insert(hostname,flowHistory);
    } else {
	// append scan flow values
	while (i != scanHashListFlow.end() && i.key() == hostname) {
	    QStringList flowHistory = i.value();
	    flowHistory.append(lineData);
	    foreach (const QString &token, flowHistory) {
		qDebug() << "DEBUG:: scan RELOAD:: " << token;
	    }
	    scanHashListFlow.insert(i.key(),flowHistory);
	    ++i;
	}
    }
    
    // search hostname on treeWidget and update data rows (index = 2)
    // take only remaining time and remove character unused, only [remaining || ETA]
    if (lineData.contains("remaining") || lineData.contains("ETC")) {
	QString infoTmp_ = lineData.mid(lineData.indexOf("("),lineData.indexOf(")"));
	infoTmp_ = infoTmp_.remove('(');
	infoTmp_ = infoTmp_.remove(')');
	// insert new information into monitor
	monitorElem[monitorElemHost.indexOf(hostname)]->setText(2,infoTmp_);
	monitorElemState[monitorElemHost.indexOf(hostname)] = infoTmp_;
    }
}

void nmapClass::monitorRuntimeEvent()
{
    if (!monitorStopCurrentScanButt->isEnabled()) {
	monitorStopCurrentScanButt->setEnabled(true);
    }
}

void nmapClass::monitorStopCurrentScan()
{
    // Stop and wait thread fron QHash table
    if (scanMonitor->selectedItems().isEmpty()) {
	return;
    }
    // TODO:: stop digSupport and internalLooup
    scanThread *ptrTmp = scanHashList.take(scanMonitor->selectedItems()[0]->text(0));
    
    if (ptrTmp) {
	ptrTmp->quit();
	ptrTmp->wait();
	delete ptrTmp;
    }
    
    // Remove Qhash entry for flow
    scanHashListFlow.take(scanMonitor->selectedItems()[0]->text(0));
}
