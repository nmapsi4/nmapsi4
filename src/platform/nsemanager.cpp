/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nsemanager.h"
#include "profilermanager.h"

NseManager::NseManager(ProfilerManager* parent)
    : QObject(parent), m_ui(parent)
{
    nseTreeDefaultValue();
}

NseManager::~NseManager()
{
    memory::freemap<QString, QTextDocument*>::itemDeleteAll(m_nseHelpCache);

    if (m_documentScript) {
        delete m_documentScript;
    }
}

const QStringList NseManager::getActiveNseScript()
{
    return m_nseScriptActiveList;
}

void NseManager::requestNseHelp(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    if (m_nseScriptAvailList.indexOf(item->text(0)) != -1) {
        m_ui->m_dialogUi->nseActiveBut->setEnabled(true);
        m_ui->m_dialogUi->nseRemoveBut->setEnabled(false);
    } else {
        m_ui->m_dialogUi->nseActiveBut->setEnabled(false);
        m_ui->m_dialogUi->nseRemoveBut->setEnabled(true);
    }
    // search nse category on nse Cache
    QHash<QString, QTextDocument*>::const_iterator i = m_nseHelpCache.find(item->text(0));

    if (i == m_nseHelpCache.constEnd()) {
        /*
        * not category on cache
        * start help thread for nse
        */
        QStringList parameters_;
        parameters_.append("--script-help");
        parameters_.append(item->text(0));

        m_thread = new ProcessThread("nmap", parameters_);

        connect(m_thread, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
                this, SLOT(showNseHelp(QStringList,QByteArray,QByteArray)));

        m_thread->start();
    } else {
        // category on cache
        qDebug() << "DEBUG:: load help from cache";
        m_ui->m_dialogUi->nseTextHelp->setDocument(i.value());
    }
}

void NseManager::requestNseScriptHelp()
{
    QString searchString_ = m_ui->m_dialogUi->comboScriptHelp->currentText();
    if (searchString_.isEmpty()) {
        return;
    }

    QStringList parameters_;
    parameters_.append("--script-help");
    parameters_.append(searchString_);

    m_threadScript = new ProcessThread("nmap", parameters_);

    connect(m_threadScript, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
            this, SLOT(showNseScriptHelp(QStringList,QByteArray,QByteArray)));

    m_threadScript->start();
}

void NseManager::showNseHelp(const QStringList parameters, QByteArray result, QByteArray errors)
{
    Q_UNUSED(errors);
    // show help result for nse
    if (m_thread) {
        m_thread->quit();
        m_thread->wait();
        delete m_thread;
    }

    QString result_(result);
    QTextDocument *document = new QTextDocument(result_);
    // insert document on cache
    m_nseHelpCache.insert(parameters[parameters.size() - 1], document);
    // load document
    m_ui->m_dialogUi->nseTextHelp->setDocument(document);
}

void NseManager::showNseScriptHelp(const QStringList parameters, QByteArray result, QByteArray errors)
{
    // result for script search
    Q_UNUSED(errors);
    Q_UNUSED(parameters);
    // show help result for nse
    if (m_threadScript) {
        m_threadScript->quit();
        m_threadScript->wait();
        delete m_threadScript;
    }

    QString result_(result);

    if (m_documentScript) {
        delete m_documentScript;
    }

    m_documentScript = new QTextDocument(result_);
    m_ui->m_dialogUi->textScriptHelp->setDocument(m_documentScript);
}

void NseManager::nseTreeDefaultValue()
{
    m_nseScriptActiveList.clear();
    m_nseScriptAvailList.clear();
    m_nseScriptAvailList.append("auth");
    m_nseScriptAvailList.append("broadcast");
    m_nseScriptAvailList.append("brute");
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

void NseManager::nseTreeAvailRestoreValues()
{
    if (m_itemNseAvail.size()) {
        memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemNseAvail);
    }

    foreach(const QString & token, m_nseScriptAvailList) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_dialogUi->nseTreeAvail);
        m_itemNseAvail.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-typedef.png")));
        root->setText(0, token);
        //root->setToolTip(0, token);
    }
}

void NseManager::nseTreeActiveRestoreValues()
{
    if (m_itemNseActive.size()) {
        memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemNseActive);
    }

    foreach(const QString & token, m_nseScriptActiveList) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_dialogUi->nseTreeActive);
        m_itemNseActive.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-function.png")));
        root->setText(0, token);
        //root->setToolTip(0, token);
    }
}

void NseManager::nseTreeActiveItem()
{
    int indexNseItem = m_ui->m_dialogUi->nseTreeAvail->indexOfTopLevelItem(m_ui->m_dialogUi->nseTreeAvail->currentItem());

    if (indexNseItem != -1) {
        QString tmpElem_ = m_nseScriptAvailList.takeAt(indexNseItem);
        m_nseScriptActiveList.append(tmpElem_);
        nseTreeAvailRestoreValues();
        nseTreeActiveRestoreValues();
        if (!m_nseScriptAvailList.size()) {
            m_nseScriptAvailList.clear();
        }
    }
}

void NseManager::nseTreeRemoveItem()
{
    int indexNseItem = m_ui->m_dialogUi->nseTreeActive->indexOfTopLevelItem(m_ui->m_dialogUi->nseTreeActive->currentItem());

    if (indexNseItem != -1) {
        QString tmpElem_ = m_nseScriptActiveList.takeAt(indexNseItem);
        m_nseScriptAvailList.append(tmpElem_);
        nseTreeAvailRestoreValues();
        nseTreeActiveRestoreValues();
        if (!m_nseScriptActiveList.size()) {
            m_nseScriptActiveList.clear();
        }
    }
}

void NseManager::nseTreeResetItem()
{
    foreach(const QString & token, m_nseScriptActiveList) {
        m_nseScriptAvailList.append(token);
    }
    m_nseScriptActiveList.clear();
    nseTreeAvailRestoreValues();
    nseTreeActiveRestoreValues();
}

void NseManager::nseTreeActiveScriptValues(const QStringList scripts)
{
    foreach(const QString & script, scripts) {
        nseTreeActiveSingleScript(script);
    }
}

bool NseManager::nseTreeActiveSingleScript(const QString script)
{
    if (!m_nseScriptAvailList.contains(script)) {
        return false;
    }

    m_nseScriptActiveList.append(m_nseScriptAvailList.takeAt(m_nseScriptAvailList.indexOf(script)));

    nseTreeAvailRestoreValues();
    nseTreeActiveRestoreValues();

    if (!m_nseScriptAvailList.size()) {
        m_nseScriptAvailList.clear();
    }

    return true;
}
