/***************************************************************************
 *   Copyright (C) 2011-2012 by Francesco Cecconi                          *
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
#include "profilerdialog.h"

nseManager::nseManager(profilerManager* parent) : QObject(parent), m_ui(parent)
{
    nseTreeDefaultValue();
}

nseManager::~nseManager()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemNseActive);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemNseAvail);
}

const QStringList nseManager::getActiveNseScript()
{
    return m_nseScriptActiveList;
}

void nseManager::requestNseHelp(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    qDebug() << "DEBUG:: item: " << item->text(0);

    if (m_nseScriptAvailList.indexOf(item->text(0)) != -1)
    {
        m_ui->nseActiveBut->setEnabled(true);
        m_ui->nseRemoveBut->setEnabled(false);
    }
    else
    {
        m_ui->nseActiveBut->setEnabled(false);
        m_ui->nseRemoveBut->setEnabled(true);
    }
    // search nse category on nse Cache
    QHash<QString, QTextDocument*>::const_iterator i = m_nseHelpCache.find(item->text(0));

    if (i == m_nseHelpCache.constEnd())
    {
        /*
        * not category on cache
        * start help thread for nse
        */
        QStringList parameters_;
        parameters_.append("--script-help");
        parameters_.append(item->text(0));

        m_thread = new QProcessThread("nmap",parameters_);

        connect(m_thread.data(), SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
                this, SLOT(showNseHelp(QStringList,QByteArray,QByteArray)));

        m_thread.data()->start();
    }
    else
    {
        // category on cache
        qDebug() << "DEBUG:: load help from cache";
        m_ui->nseTextHelp->setDocument(i.value());
    }
}

void nseManager::requestNseScriptHelp()
{
    QString searchString_ = m_ui->comboScriptHelp->currentText();
    if (searchString_.isEmpty())
    {
        return;
    }

    QStringList parameters_;
    parameters_.append("--script-help");
    parameters_.append(searchString_);

    m_threadScript = new QProcessThread("nmap",parameters_);

    connect(m_threadScript.data(), SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
            this, SLOT(showNseScriptHelp(QStringList,QByteArray,QByteArray)));

    m_threadScript.data()->start();
}

void nseManager::showNseHelp(const QStringList parameters, QByteArray result, QByteArray errors)
{
    Q_UNUSED(errors);
    // show help result for nse
    m_thread.data()->quit();
    m_thread.data()->wait();
    delete m_thread.data();

    QString result_(result);
    QTextDocument *document = new QTextDocument(result_);
    // insert document on cache
    m_nseHelpCache.insert(parameters[parameters.size()-1],document);
    // load document
    m_ui->nseTextHelp->setDocument(document);
}

void nseManager::showNseScriptHelp(const QStringList parameters, QByteArray result, QByteArray errors)
{
    // result for script search
    Q_UNUSED(errors);
    Q_UNUSED(parameters);
    // show help result for nse
    m_threadScript.data()->quit();
    m_threadScript.data()->wait();
    delete m_threadScript.data();

    QString result_(result);

    if (!m_documentScript.isNull())
    {
        qDebug() << "DEBUG::ScriptNse delete document";
        delete m_documentScript.data();
    }

    m_documentScript = new QTextDocument(result_);
    m_ui->textScriptHelp->setDocument(m_documentScript.data());
}

void nseManager::nseTreeDefaultValue()
{
    m_nseScriptActiveList.clear();
    m_nseScriptAvailList.clear();
    m_nseScriptAvailList.append("auth");
    m_nseScriptAvailList.append("default");
    m_nseScriptAvailList.append("discovery");
    m_nseScriptAvailList.append("dos");
    m_nseScriptAvailList.append("exploit");
    m_nseScriptAvailList.append("external");
    m_nseScriptAvailList.append("fuzzer");
    m_nseScriptAvailList.append("intrusive");
    m_nseScriptAvailList.append("malware");
    m_nseScriptAvailList.append("safe");
    m_nseScriptAvailList.append("version");
    m_nseScriptAvailList.append("vuln");

    nseTreeAvailRestoreValues();
}

void nseManager::nseTreeAvailRestoreValues()
{
    if (m_itemNseAvail.size())
    {
        freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemNseAvail);
    }

    foreach (const QString &token, m_nseScriptAvailList)
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->nseTreeAvail);
        m_itemNseAvail.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-typedef.png")));
        root->setText(0, token);
        //root->setToolTip(0, token);
    }
}

void nseManager::nseTreeActiveRestoreValues()
{
    if (m_itemNseActive.size())
    {
        freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemNseActive);
    }

    foreach (const QString &token, m_nseScriptActiveList)
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->nseTreeActive);
        m_itemNseActive.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-function.png")));
        root->setText(0, token);
        //root->setToolTip(0, token);
    }
}

void nseManager::nseTreeActiveItem()
{
    int indexNseItem = m_ui->nseTreeAvail->indexOfTopLevelItem(m_ui->nseTreeAvail->currentItem());

    if (indexNseItem != -1)
    {
        QString tmpElem_ = m_nseScriptAvailList.takeAt(indexNseItem);
        m_nseScriptActiveList.append(tmpElem_);
        nseTreeAvailRestoreValues();
        nseTreeActiveRestoreValues();
        if (!m_nseScriptActiveList.size())
        {
            m_nseScriptActiveList.clear();
        }
    }
}

void nseManager::nseTreeRemoveItem()
{
    int indexNseItem = m_ui->nseTreeActive->indexOfTopLevelItem(m_ui->nseTreeActive->currentItem());

    if (indexNseItem != -1)
    {
        QString tmpElem_ = m_nseScriptActiveList.takeAt(indexNseItem);
        m_nseScriptAvailList.append(tmpElem_);
        nseTreeAvailRestoreValues();
        nseTreeActiveRestoreValues();
        if (!m_nseScriptAvailList.size())
        {
            m_nseScriptAvailList.clear();
        }
    }
}

void nseManager::nseTreeResetItem()
{
    foreach (const QString &token, m_nseScriptActiveList)
    {
        m_nseScriptAvailList.append(token);
    }
    m_nseScriptActiveList.clear();
    nseTreeAvailRestoreValues();
    nseTreeActiveRestoreValues();
}
