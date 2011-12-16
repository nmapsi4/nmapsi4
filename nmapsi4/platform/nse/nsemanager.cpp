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

#include "nsemanager.h"
#include "mainwin.h"

nseManager::nseManager(nmapClass* parent) : QObject(parent), _ui(parent)
{
}

nseManager::~nseManager()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(itemNseActive);
    freelist<QTreeWidgetItem*>::itemDeleteAll(itemNseAvail);
}

void nseManager::loadNseCategoryScript()
{
    QSettings settings("nmapsi4", "nmapsi4");

    int nseComboScriptTmp_ = settings.value("nseComboScript", 0).toInt();
    updateNseOptionScript(nseComboScriptTmp_);

    nseScriptActiveList = settings.value("nseScriptActiveList","none").toStringList();
    nseScriptAvailList = settings.value("nseScriptAvailList","none").toStringList();

    if (!nseScriptAvailList.first().compare("none"))
    {
        nseTreeDefaultValue();
    }
    else
    {
        if (!nseScriptAvailList.first().compare(""))
        {
            nseScriptAvailList.removeFirst();
        }

        if (!nseScriptActiveList.first().compare(""))
        {
            nseScriptActiveList.removeFirst();
        }

        nseTreeAvailRestoreValues();
        nseTreeActiveRestoreValues();
    }
}

const QStringList nseManager::getActiveNseScript()
{
    return nseScriptActiveList;
}

void nseManager::sync()
{
    /**
    * Save script list state to disk
    **/

    QSettings settings("nmapsi4", "nmapsi4");

    if (nseScriptActiveList.isEmpty())
    {
        settings.setValue("nseScriptActiveList","");
    }
    else
    {
        settings.setValue("nseScriptActiveList",QVariant(nseScriptActiveList));
    }

    if (nseScriptAvailList.isEmpty())
    {
        settings.setValue("nseScriptAvailList", "");
    }
    else
    {
        settings.setValue("nseScriptAvailList", QVariant(nseScriptAvailList));
    }
}

void nseManager::requestNseHelp(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    qDebug() << "DEBUG:: item: " << item->text(0);

    if (nseScriptAvailList.indexOf(item->text(0)) != -1)
    {
        _ui->nseActiveBut->setEnabled(true);
        _ui->nseRemoveBut->setEnabled(false);
    }
    else
    {
        _ui->nseActiveBut->setEnabled(false);
        _ui->nseRemoveBut->setEnabled(true);
    }
    // search nse category on nse Cache
    QHash<QString, QTextDocument*>::const_iterator i = nseHelpCache.find(item->text(0));

    if (i == nseHelpCache.constEnd())
    {
        /*
        * not category on cache
        * start help thread for nse
        */
        QStringList parameters_;
        parameters_.append("--script-help");
        parameters_.append(item->text(0));

        _thread = new QProcessThread("nmap",parameters_);

        connect(_thread, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
                this, SLOT(showNseHelp(QStringList,QByteArray,QByteArray)));

        _thread->start();
    }
    else
    {
        // category on cache
        qDebug() << "DEBUG:: load help from cache";
        _ui->nseTextHelp->setDocument(i.value());
    }
}

void nseManager::requestNseScriptHelp()
{
    QString searchString_ = _ui->comboScriptHelp->currentText();
    if (searchString_.isEmpty())
    {
        return;
    }

    QStringList parameters_;
    parameters_.append("--script-help");
    parameters_.append(searchString_);

    _threadScript = new QProcessThread("nmap",parameters_);

    connect(_threadScript, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
            this, SLOT(showNseScriptHelp(QStringList,QByteArray,QByteArray)));

    _threadScript->start();
}

void nseManager::showNseHelp(const QStringList parameters, QByteArray result, QByteArray errors)
{
    Q_UNUSED(errors);
    // show help result for nse
    _thread->quit();
    _thread->wait();
    delete _thread;

    QString result_(result);
    QTextDocument *document = new QTextDocument(result_);
    // insert document on cache
    nseHelpCache.insert(parameters[parameters.size()-1],document);
    // load document
    _ui->nseTextHelp->setDocument(document);
}

void nseManager::showNseScriptHelp(const QStringList parameters, QByteArray result, QByteArray errors)
{
    // result for script search
    Q_UNUSED(errors);
    Q_UNUSED(parameters);
    // show help result for nse
    _threadScript->quit();
    _threadScript->wait();
    delete _threadScript;

    QString result_(result);

    if (_documentScript)
    {
        qDebug() << "DEBUG::ScriptNse delete document";
        delete _documentScript;
    }

    _documentScript = new QTextDocument(result_);
    _ui->textScriptHelp->setDocument(_documentScript);
}

void nseManager::nseTreeDefaultValue()
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

void nseManager::nseTreeAvailRestoreValues()
{
    if (itemNseAvail.size())
    {
        freelist<QTreeWidgetItem*>::itemDeleteAll(itemNseAvail);
        itemNseAvail.clear();
    }

    foreach (const QString &token, nseScriptAvailList)
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->nseTreeAvail);
        itemNseAvail.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-typedef.png")));
        root->setText(0, token);
        //root->setToolTip(0, token);
    }
}

void nseManager::nseTreeActiveRestoreValues()
{
    if (itemNseActive.size())
    {
        freelist<QTreeWidgetItem*>::itemDeleteAll(itemNseActive);
        itemNseActive.clear();
    }

    foreach (const QString &token, nseScriptActiveList)
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->nseTreeActive);
        itemNseActive.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-function.png")));
        root->setText(0, token);
        //root->setToolTip(0, token);
    }
}

void nseManager::nseTreeActiveItem()
{
    int indexNseItem = _ui->nseTreeAvail->indexOfTopLevelItem(_ui->nseTreeAvail->currentItem());

    if (indexNseItem != -1)
    {
        QString tmpElem_ = nseScriptAvailList.takeAt(indexNseItem);
        nseScriptActiveList.append(tmpElem_);
        nseTreeAvailRestoreValues();
        nseTreeActiveRestoreValues();
        if (!nseScriptActiveList.size())
        {
            nseScriptActiveList.clear();
        }
    }

    if (_ui->_collectionsButton.value("nss-act")->isChecked())
    {
        _ui->comboAdv->clear();
        _ui->comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
        _ui->comboAdv->insertItem(0, _ui->check_extensions().join(" "));
    }
}

void nseManager::nseTreeRemoveItem()
{
    int indexNseItem = _ui->nseTreeActive->indexOfTopLevelItem(_ui->nseTreeActive->currentItem());

    if (indexNseItem != -1)
    {
        QString tmpElem_ = nseScriptActiveList.takeAt(indexNseItem);
        nseScriptAvailList.append(tmpElem_);
        nseTreeAvailRestoreValues();
        nseTreeActiveRestoreValues();
        if (!nseScriptAvailList.size())
        {
            nseScriptAvailList.clear();
        }
    }

    if (_ui->_collectionsButton.value("nss-act")->isChecked())
    {
        _ui->comboAdv->clear();
        _ui->comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
        _ui->comboAdv->insertItem(0, _ui->check_extensions().join(" "));
    }
}

void nseManager::nseTreeResetItem()
{
    foreach (const QString &token, nseScriptActiveList)
    {
        nseScriptAvailList.append(token);
    }
    nseScriptActiveList.clear();
    nseTreeAvailRestoreValues();
    nseTreeActiveRestoreValues();
}

void nseManager::updateNseOptionScript(int index)
{
    _ui->nseComboScript->setCurrentIndex(index);

    if (index)
    {
        _ui->nseTreeActive->setEnabled(true);
        _ui->nseTreeAvail->setEnabled(true);
        _ui->nseResetBut->setEnabled(true);
        _ui->comboNseInv->setEnabled(true);
        _ui->comboNsePar->setEnabled(true);
        _ui->nseFixedSButt->setEnabled(true);
    }
    else
    {
        _ui->nseTreeActive->setEnabled(false);
        _ui->nseTreeAvail->setEnabled(false);
        _ui->nseResetBut->setEnabled(false);
        _ui->nseActiveBut->setEnabled(false);
        _ui->nseRemoveBut->setEnabled(false);
        _ui->comboNseInv->setEnabled(false);
        _ui->comboNsePar->setEnabled(false);
        _ui->nseFixedSButt->setEnabled(false);
    }
    // reset parameters for change
    _ui->resetPar();
}
