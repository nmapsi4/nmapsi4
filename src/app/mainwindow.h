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
#include <QtGui/QCloseEvent>
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
    void preScanLookup(const QString hostname);
    void setNmapsiSlot();
    void resetOptions();
    void initGUI();
    void setTreeWidgetValues();
    void createBar();
    void loadScanProfile();
    void updateComboBook();
    void restoreGlobalProfile();
    void loadHistoryDefault();
    void restoreSettings();
    void setTreeSettings();
    void setDefaultAction();
    void setDefaultSplitter();
    void defaultComboValues();
    void createToolButtonSetup();
    void updateCompleter();
    bool checkViewOS(const QString OSline, QTreeWidgetItem *itemOS) const;
    QStringList loadExtensions();
    QHash<QString, QString> defaultScanProfile() const;

    QHash<QString, QPushButtonOrientated*> m_collectionsButton;
    QSplitter *m_mainHorizontalSplitter;
    QSplitter *m_mainVerticalSplitter;
    QToolButton *m_menuSetup;
    QToolButton *m_profilerTool;
    QToolButton *m_saveTool;
    QToolButton *m_bookmarksTool;
    QWeakPointer<QCompleter> m_completer;
    QWeakPointer<QCompleter> m_completerVuln;
    QWeakPointer<QStringListModel> m_hostModel;
    QWeakPointer<QStringListModel> m_vulnModel;
    vulnerability* m_vulnerability;
    discoverManager* m_discoverManager;
    monitor* m_monitor;
    utilities* m_utilities;
    parserManager* m_parser;
    QList<QTreeWidgetItem*> m_treeloghlist;
    QList<QTreeWidgetItem*> m_treebookparlist;
    QList<QTreeWidgetItem*> m_treebookvulnlist;
    QList<QTreeWidgetItem*> m_treewidgetvulnlist;
    int m_userId;
    int m_hostCache;
    int m_lookupType;
    bool m_flag_state;
    bool m_savePos;
    bool m_saveSize;
    bool m_TraceEnabled;
    bool m_lookupEnabled;

private slots:
    void initObject();
    void startScan();
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

protected:
    /**
     * Reimplement closeEvent protected function
     */
    virtual void closeEvent(QCloseEvent * event);
};

#endif
