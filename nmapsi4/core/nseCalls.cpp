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

namespace localCall {   
    QPointer<scanThread> th;
}

void nmapClass::updateNseOptionScript(int index)
{
    nseComboScript->setCurrentIndex(index);

    if (index) {
        nseTreeActive->setEnabled(true);
        nseTreeAvail->setEnabled(true);
	nseResetBut->setEnabled(true);
	comboNseInv->setEnabled(true);
	comboNsePar->setEnabled(true);
	nseFixedSButt->setEnabled(true);
    } else {
        nseTreeActive->setEnabled(false);
        nseTreeAvail->setEnabled(false);
	nseResetBut->setEnabled(false);
	nseActiveBut->setEnabled(false);
	nseRemoveBut->setEnabled(false);
	comboNseInv->setEnabled(false);
	comboNsePar->setEnabled(false);
	nseFixedSButt->setEnabled(false);
    }
    // reset parameters for change
    resetPar();
}

void nmapClass::nseTreeDefaultValue()
{
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

void nmapClass::nseTreeAvailRestoreValues()
{
    if (itemNseAvail.size()) {
        memoryTools *memTools = new memoryTools();
        memTools->itemDeleteAll(itemNseAvail);
        delete memTools;
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

void nmapClass::nseTreeActiveRestoreValues()
{
    if (itemNseActive.size()) {
        memoryTools *memTools = new memoryTools();
        memTools->itemDeleteAll(itemNseActive);
        delete memTools;
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

void nmapClass::nseTreeActiveItem()
{
    int indexNseItem = nseTreeAvail->indexOfTopLevelItem(nseTreeAvail->currentItem());

    if (indexNseItem != -1) {
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

void nmapClass::nseTreeRemoveItem()
{
    int indexNseItem = nseTreeActive->indexOfTopLevelItem(nseTreeActive->currentItem());

    if (indexNseItem != -1) {
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

void nmapClass::nseTreeResetItem()
{
    foreach (const QString &token, nseScriptActiveList) {
        nseScriptAvailList.append(token);
    }
    nseScriptActiveList.clear();
    nseTreeAvailRestoreValues();
    nseTreeActiveRestoreValues();
}

void nmapClass::requestNseHelp(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    qDebug() << "DEBUG:: item: " << item->text(0);

    if (nseScriptAvailList.indexOf(item->text(0)) != -1) {
	nseActiveBut->setEnabled(true);
	nseRemoveBut->setEnabled(false);
    } else {
	nseActiveBut->setEnabled(false);
	nseRemoveBut->setEnabled(true);
    }
    // search nse category on nse Cache
    QHash<QString, QTextDocument*>::const_iterator i = nseHelpCache.find(item->text(0));
    
    if (i == nseHelpCache.end()) {
	/*
	 * not category on cache
	 * start help thread for nse
	 */
	QByteArray buff1;
	QByteArray buff2;

	QStringList parameters_;
	parameters_.append("--script-help");
	parameters_.append(item->text(0));

	localCall::th = new scanThread(buff1, buff2, parameters_, this);

	connect(localCall::th, SIGNAL(threadEnd(const QStringList, QByteArray, QByteArray)),
		this, SLOT(showNseHelp(QStringList,QByteArray,QByteArray)));

	localCall::th->start();
    } else {
	// category on cache
	qDebug() << "DEBUG:: load help from cache";
	nseTextHelp->setDocument(i.value());
    }
}

void nmapClass::showNseHelp(const QStringList parameters, QByteArray result, QByteArray errors)
{
    Q_UNUSED(errors);
    // show help result for nse
    localCall::th->quit();
    localCall::th->wait();
    delete localCall::th;

    QString result_(result);
    QTextDocument *document = new QTextDocument(result_);
    // insert document on cache
    nseHelpCache.insert(parameters[parameters.size()-1],document);
    // load document
    nseTextHelp->setDocument(document);
}
