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

#ifndef NSEMANAGER_H
#define NSEMANAGER_H

#include <QtCore/QObject>
#include <QtGui/QTextDocument>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QSettings>
#include <QtCore/QPointer>
#include <QtWebKit/QWebView>

#include "qprocessthread.h"

class nmapClass;

class nseManager : public QObject
{
    Q_OBJECT

public:
    nseManager(nmapClass* parent);
    ~nseManager();
    /**
     * Save script list state to file
     *
     **/
    void sync();
    /**
     * Load script list state from file
     *
     **/
    void loadNseCategoryScript();
    /**
     * Get QString List with all actived script
     *
     **/
    const QStringList getActiveNseScript();

private:
    void nseTreeAvailRestoreValues();
    void nseTreeActiveRestoreValues();

    nmapClass* m_ui;
    QPointer<QProcessThread> m_thread;
    QPointer<QProcessThread> m_threadScript;
    QPointer<QTextDocument> m_documentScript;
    QList<QTreeWidgetItem*> m_itemNseAvail;
    QList<QTreeWidgetItem*> m_itemNseActive;
    QHash<QString, QTextDocument*> m_nseHelpCache;
    QStringList m_nseScriptAvailList;
    QStringList m_nseScriptActiveList;

public slots:
    void requestNseHelp(QTreeWidgetItem *item, int column);
    void requestNseScriptHelp();
    void showNseHelp(const QStringList parameters, QByteArray result, QByteArray errors);
    void showNseScriptHelp(const QStringList parameters, QByteArray result, QByteArray errors);
    void nseTreeDefaultValue();
    void nseTreeActiveItem();
    void nseTreeRemoveItem();
    void nseTreeResetItem();
    void updateNseOptionScript(int index);
};

#endif // NSEMANAGER_H
