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

#ifndef NSEMANAGER_H
#define NSEMANAGER_H

#include <QtCore/QObject>
#include <QtGui/QTextDocument>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QSettings>
#include <QtCore/QWeakPointer>
#include <QtWebKit/QWebView>

#include "processthread.h"
#include "memorytools.h"

class ProfilerManager;

class NseManager : public QObject
{
    Q_OBJECT

public:
    explicit NseManager(ProfilerManager* parent);
    ~NseManager();
    /**
     * Get QString List with all actived script
     *
     **/
    const QStringList getActiveNseScript();
    void nseTreeActiveScriptValues(const QStringList scripts);
    bool nseTreeActiveSingleScript(const QString script);

private:
    void nseTreeAvailRestoreValues();
    void nseTreeActiveRestoreValues();

    ProfilerManager* m_ui;
    QWeakPointer<ProcessThread> m_thread;
    QWeakPointer<ProcessThread> m_threadScript;
    QWeakPointer<QTextDocument> m_documentScript;
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

public slots:
    void nseTreeResetItem();
};

#endif // NSEMANAGER_H
