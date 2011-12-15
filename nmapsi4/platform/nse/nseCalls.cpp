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

#include "mainwin.h"

/**
 *
 * TODO: create nse class
 *
 **/

namespace localCall {
    QPointer<QProcessThread> th;
    QPointer<QProcessThread> thScript;
    QTextDocument *documentScript = NULL;
}

void nmapClass::requestNseHelp(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    qDebug() << "DEBUG:: item: " << item->text(0);

    if (nseScriptAvailList.indexOf(item->text(0)) != -1)
    {
        nseActiveBut->setEnabled(true);
        nseRemoveBut->setEnabled(false);
    }
    else
    {
        nseActiveBut->setEnabled(false);
        nseRemoveBut->setEnabled(true);
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

        localCall::th = new QProcessThread("nmap",parameters_);

        connect(localCall::th, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
                this, SLOT(showNseHelp(QStringList,QByteArray,QByteArray)));

        localCall::th->start();
    }
    else
    {
        // category on cache
        qDebug() << "DEBUG:: load help from cache";
        nseTextHelp->setDocument(i.value());
    }
}

void nmapClass::requestNseScriptHelp()
{
    QString searchString_ = comboScriptHelp->currentText();
    if (searchString_.isEmpty())
    {
        return;
    }

    QStringList parameters_;
    parameters_.append("--script-help");
    parameters_.append(searchString_);

    localCall::thScript = new QProcessThread("nmap",parameters_);

    connect(localCall::thScript, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
            this, SLOT(showNseScriptHelp(QStringList,QByteArray,QByteArray)));

    localCall::thScript->start();
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

void nmapClass::showNseScriptHelp(const QStringList parameters, QByteArray result, QByteArray errors)
{
    // result for script search
    Q_UNUSED(errors);
    Q_UNUSED(parameters);
    // show help result for nse
    localCall::thScript->quit();
    localCall::thScript->wait();
    delete localCall::thScript;

    QString result_(result);

    if (localCall::documentScript != NULL)
    {
        qDebug() << "DEBUG::ScriptNse delete document";
        delete localCall::documentScript;
    }

    localCall::documentScript = new QTextDocument(result_);
    textScriptHelp->setDocument(localCall::documentScript);
}
