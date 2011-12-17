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

#ifndef MAINWIN_H
#define MAINWIN_H

#include "ui_mainwin.h"

// Qt4 include
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QToolButton>
#include <QtGui/QTextDocument>
#include <QtGui/QSplitter>
#include <QtCore/QHash>
#include <QtCore/QPointer>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>
#include <QtWebKit/QWebView>
#include <QtGui/QCompleter>
#include <QtGui/QStringListModel>

// local include
#include "profilemain.h"
#include "loghistory.h"
#include "staticDefine.h"
#include "nmapsi4Debug.h"
#include "lookupmanager.h"
#include "addparameterstobookmark.h"
#include "memorytools.h"
#include "monitor.h"
#include "utilities.h"
#include "hostTools.h"
#include "qpushbuttonorientated.h"
#include "vulnerability.h"
#include "nsemanager.h"
#include "discovermanager.h"
#include "parsermanager.h"
#include "digmanager.h"

// define class namespace
using namespace memory;

namespace Ui
{
    class MainWindow;
}

class nmapClass : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    friend class parserManager;
    friend class vulnerability;
    friend class nseManager;
    friend class discoverManager;

public:
    nmapClass();
    ~nmapClass();

private:
    void preScanLookup(const QString hostname);
    void rootMode();
    void isEmptyLog();
    void setNmapsiSlot();
    QFile* create_logFile(const QString Path);
    QStringList check_extensions();
    QStringList loadExtensions();
    void resetOptions();
    void checkProfile();
    void initGUI();
    bool checkViewOS(const QString OSline, QTreeWidgetItem *itemOS) const;
    void setTreeWidgetValues();
    void createBar();
    void updateComboPar();
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
    void setQuickProfile();
    void setNormalProfile();
    void setFullVersionProfile();
    void setQuickVersionProfile();
    void updateCompleter();

protected:
    int uid;
    QFile *_logFilePath;
    QListWidgetItem *scanW;
    QListWidgetItem *toolW;
    QListWidgetItem *discoverW;
    QListWidgetItem *fileW;
    QListWidgetItem *timingW;
    QString FileName;
    QString firstPath;
    QString Profile;
    QString logPath;
    QString logSessionFile;
    bool globalProfile;
    bool checkLog;
    bool flag_state;
    bool savePos;
    bool saveSize;
    bool listClearFlag;
    bool verboseLog;
    bool lookupInternal;
    bool lookupDig;
    bool NSSsupport;
    bool TraceEnabled;
    bool LookupEnabled;
    bool digSupported;
    int hostCache;;
    QList<QTreeWidgetItem*> mainTreeElem;
    QHash<QString, QPushButtonOrientated*> _collectionsButton;
    QList<lookupManager*> internealLookupList;
    QList<digManager*> digLookupList;
    QSplitter *cW;
    QSplitter *bW;
    QToolButton *menuSetup;
    monitor *_monitor;
    utilities *_utilities;
    parserManager *_parser;
    QPointer<QCompleter> _completer;
    QPointer<QCompleter> _completerVuln;
    QPointer<QStringListModel> _hostModel;
    QPointer<QStringListModel> _vulnModel;
    vulnerability* _vulnerability;
    nseManager* _nseManager;
    discoverManager* _discoverManager;

private slots:
    void initObject();
    void startScan();
    void stop_scan();
    void exit();
    void update_portCombo();
    void update_scanCombo();
    void update_inputcheck();
    void update_discover();
    void update_timing();
    void update_options();
    void update_comboVerbosity();
    void openBrowser();
    void fileSession();
    void listClear();
    void checkFullScreen();
    void updateMenuBar();
    void updateIconsBox();
    void callScanH();
    void callParFromBook();
    void scanLookup(QHostInfo info, int state, const QString hostname);
    void monitorRuntimeEvent();
    void updateScanCounter(int hostNumber);
    //update and log slots
    void startProfile_ui();
    void readProfile();
    void saveAsLog();
    void saveLog();
    void updateFontHost();
    void callSearchHistory();
    void saveBookMarks();
    void saveBookMarksPar(const QString profileName, const QString profilePar);
    void deleteBookMark();
    void deleteBookMarkPar();
    void optionListCreate();
    void optionListUpdate();
    void updateSezScan();
    void updateSezLog();
    void updateSezVuln();
    void updateDiscoverVuln();
    void updateTabLook();
    void updateTabTrace();
    void NSSCheck();
    void loadDefaultProfile();
    void resetPar();
    void detailsOptions();
    void nssOptions();
    void bookOptions();
    void bookOptionsPar();
    //contest Menu
    void menuScanBook();
    void menuVulnBook();
    void menuParBook();
    void menuServiceMain();
    void slotParSelected();
    void slotHostSelected();
    void saveUiSettings();
    // global profile
    void saveGlobalProfile();
    void removeGlobalProfile();
    void startAddParBook_ui();
};

#endif
