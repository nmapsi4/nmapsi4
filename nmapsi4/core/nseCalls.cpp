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

#include "../mainwin.h"

void nmapClass::updateNseOptionScript(int index) {

    nseComboScript->setCurrentIndex(index);
    
    if(index) {
	nseTreeActive->setEnabled(true);
	nseTreeAvail->setEnabled(true);
	nseActiveBut->setEnabled(true);
    } else {
	nseTreeActive->setEnabled(false);
	nseTreeAvail->setEnabled(false);
	nseActiveBut->setEnabled(false);
    }
    // reset parameters for change
    resetPar();
}

void nmapClass::nseTreeDefaultValue() {
    nseScriptActiveList.clear();
    nseScriptAvailList.clear();
    nseScriptAvailList.append("auth");
    nseScriptAvailList.append("default");
    nseScriptAvailList.append("discovery");
    nseScriptAvailList.append("dos");
    nseScriptAvailList.append("exploit");
    nseScriptAvailList.append("external");
    nseScriptAvailList.append("fuzzer");
    nseScriptAvailList.append("intrusive");
    nseScriptAvailList.append("malware");
    nseScriptAvailList.append("safe");
    nseScriptAvailList.append("version");
    nseScriptAvailList.append("vuln");
    
    nseTreeAvailRestoreValues();
}

void nmapClass::nseTreeAvailRestoreValues() {

    if(itemNseAvail.size()) {
	itemDeleteAll(itemNseAvail);
	itemNseAvail.clear();
    }
    
    foreach (const QString &token, nseScriptAvailList) {
	QTreeWidgetItem *root = new QTreeWidgetItem(nseTreeAvail);
        itemNseAvail.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-typedef.png")));
        root->setText(0, token);
        //root->setToolTip(0, token);
    }
}

void nmapClass::nseTreeActiveRestoreValues() {
    
    if(itemNseActive.size()) {
	itemDeleteAll(itemNseActive);
	itemNseActive.clear();
    }
    
    foreach (const QString &token, nseScriptActiveList) {
	QTreeWidgetItem *root = new QTreeWidgetItem(nseTreeActive);
        itemNseActive.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-function.png")));
        root->setText(0, token);
        //root->setToolTip(0, token);
    }
}

void nmapClass::nseTreeActiveItem() {

    int indexNseItem = nseTreeAvail->indexOfTopLevelItem(nseTreeAvail->currentItem());

    if(indexNseItem != -1) {
	QString tmpElem_ = nseScriptAvailList.takeAt(indexNseItem);
	nseScriptActiveList.append(tmpElem_);
	nseTreeAvailRestoreValues();
	nseTreeActiveRestoreValues();
	if (!nseScriptActiveList.size()) {
	    nseScriptActiveList.clear();
	}
    }
    
    qDebug() << "DEBUG:: nss act:: " << nssAct->isChecked();
    if (nssAct->isChecked()) {
	comboAdv->clear();
	comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
	comboAdv->insertItem(0, check_extensions().join(" "));
    }
}

void nmapClass::nseTreeRemoveItem() {

    int indexNseItem = nseTreeActive->indexOfTopLevelItem(nseTreeActive->currentItem());

    if(indexNseItem != -1) {
	QString tmpElem_ = nseScriptActiveList.takeAt(indexNseItem);
	nseScriptAvailList.append(tmpElem_);
	nseTreeAvailRestoreValues();
	nseTreeActiveRestoreValues();
	if (!nseScriptAvailList.size()) {
	    nseScriptAvailList.clear();
	}
    }
    
    qDebug() << "DEBUG:: nss act:: " << nssAct->isChecked();
    if (nssAct->isChecked()) {
	comboAdv->clear();
	comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
	comboAdv->insertItem(0, check_extensions().join(" "));
    }
}

void nmapClass::nseTreeResetItem() {

    foreach (const QString &token, nseScriptActiveList) {
	nseScriptAvailList.append(token);
    }
    nseScriptActiveList.clear();
    nseTreeAvailRestoreValues();
    nseTreeActiveRestoreValues();
}
