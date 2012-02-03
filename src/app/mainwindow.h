/***************************************************************************
 *   Copyright (C) 2007-2011 by Francesco Cecconi                          *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

// Qt4 include
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QToolButton>
#include <QtGui/QSplitter>
#include <QtGui/QCompleter>
#include <QtGui/QStringListModel>
#include <QtCore/QHash>
#include <QtCore/QWeakPointer>
#include <QtCore/QTimer>

// local include
#include "preferencesdialog.h"
#include "history.h"
#include "staticDefine.h"
#include "nmapsi4Debug.h"
#include "addparameterstobookmark.h"
#include "memorytools.h"
#include "monitor.h"
#include "utilities.h"
#include "hostTools.h"
#include "qpushbuttonorientated.h"
#include "vulnerability.h"
#include "discovermanager.h"
#include "parsermanager.h"
#include "profilerdialog.h"

static const char verticalStyleSheet[] = "QPushButton:checked {border: 0px; border-radius: 2px; \
        background-color: palette(highlight); color: palette(highlighted-text); \
        margin-left: 0px; margin-right: 3px; padding: 4px;}";

// define class namespace
using namespace memory;

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow, public Ui::MainWindowClass
{
    Q_OBJECT

    friend class parserManager;
    friend class vulnerability;
    friend class discoverManager;
    friend class monitor;

public:
    MainWindow();
    ~MainWindow();

private:
    int uid;
    int hostCache;
    int lookupType;
    bool flag_state;
    bool savePos;
    bool saveSize;
    bool TraceEnabled;
    bool lookupEnabled;
    QHash<QString, QPushButtonOrientated*> _collectionsButton;
    QSplitter *cW;
    QSplitter *bW;
    QToolButton *menuSetup;
    QToolButton *m_profilerTool;
    QToolButton *m_saveTool;
    QToolButton *m_bookmarksTool;
    monitor *_monitor;
    utilities *_utilities;
    parserManager *_parser;
    QWeakPointer<QCompleter> _completer;
    QWeakPointer<QCompleter> _completerVuln;
    QWeakPointer<QStringListModel> _hostModel;
    QWeakPointer<QStringListModel> _vulnModel;
    vulnerability* _vulnerability;
    discoverManager* _discoverManager;
    QList<QTreeWidgetItem*> m_treeloghlist;
    QList<QTreeWidgetItem*> m_treebookparlist;
    QList<QTreeWidgetItem*> m_treebookvulnlist;
    QList<QTreeWidgetItem*> m_treewidgetvulnlist;

    void preScanLookup(const QString hostname);
    void setNmapsiSlot();
    QStringList loadExtensions();
    void resetOptions();
    void initGUI();
    bool checkViewOS(const QString OSline, QTreeWidgetItem *itemOS) const;
    void setTreeWidgetValues();
    void createBar();
    void loadScanProfile();
    void updateComboBook();
    void restoreGlobalProfile();
    QHash<QString, QString> defaultScanProfile() const;
    void loadHistoryDefault();
    void restoreSettings();
    void setTreeSettings();
    void setDefaultAction();
    void setDefaultSplitter();
    void defaultComboValues();
    void createToolButtonSetup();
    void updateCompleter();

private slots:
    void initObject();
    void startScan();
    void exit();
    void listClear();
    void checkFullScreen();
    void updateMenuBar();
    void updateIconsBox();
    void callScanH();
    void monitorRuntimeEvent();
    void updateScanCounter(int hostNumber);
    void startProfile_ui();
    void syncSettings();
    void updateFontHost();
    void callSearchHistory();
    void saveBookMarks();
    void saveBookMarksPar(const QString profileName, const QString profilePar);
    void deleteBookMark();
    void deleteBookMarkPar();
    void updateSezScan();
    void updateSezVuln();
    void updateSezDiscover();
    void updateTabLook();
    void updateTabTrace();
    void loadDefaultProfile();
    void resetPar();
    void detailsOptions();
    void nssOptions();
    void menuScanBook();
    void menuVulnBook();
    void menuParBook();
    void menuServiceMain();
    void menuVulnUrlBookmark();
    void slotParSelected();
    void slotHostSelected();
    void saveSettings();
    void startAddParBook_ui();
    void startProfilerManager();
    void editProfile();
    void clearHostnameCombo();
    void clearParametersCombo();
};

#endif
