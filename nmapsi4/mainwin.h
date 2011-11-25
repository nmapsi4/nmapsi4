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

// local include
#include "profilemain.h"
#include "loghistory.h"
#include "staticDefine.h"
#include "nmapsi4Debug.h"
#include "lookUpT.h"
#include "digSupport.h"
#include "parserObj.h"
#include "parserObjUtil.h"
#include "addurl.h"
#include "maindiscover.h"
#include "addparbook.h"
#include "memorytools.h"
#include "monitor.h"
#include "utilities.h"
#include "hostTools.h"
#include "qpushbuttonorientated.h"

// define class namespace
using namespace internalLookup;
using namespace scanning;
using namespace digInterface;
using namespace parserObject;
using namespace parserUtilObject;
using namespace pingInterface;
using namespace memory;

class nmapClass : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

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
    void showParserObj(int hostIndex);
    void showParserObjPlugins(int hostIndex);
    void setTreeWidgetValues();
    void createBar();
    void updateComboPar();
    void updateComboWebV();
    void updateComboBook();
    void restoreGlobalProfile();
    QHash<QString, QString> defaultScanProfile() const;
    QHash<QString, QString> defaultUrlVuln() const;
    void startDiscover();
    void loadHistoryDefault();
    void restoreSettings();
    void setTreeSettings();
    void setDefaultAction();
    void setDefaultSplitter();
    void defaultComboValues();
    void defaultDiscoverProbes();
    void createToolButtonSetup();
    void setQuickProfile();
    void setNormalProfile();
    void setFullVersionProfile();
    void setQuickVersionProfile();
    parserObj* parser(const QStringList parList, QString StdoutStr,
                      QString StderrorStr, QTreeWidgetItem* mainTreeE);

signals:
    void killDiscover();

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
    bool ADVSupport;
    bool MonitorEnabled;
    bool TraceEnabled;
    bool LookupEnabled;
    bool digSupported;
    int hostCache;
    QList<QTreeWidgetItem*> itemListScan;
    QList<QTreeWidgetItem*> itemNseAvail;
    QList<QTreeWidgetItem*> itemNseActive;
    QList<QTreeWidgetItem*> objElem;
    QList<QTreeWidgetItem*> mainTreeElem;
    QList<QString> nseScriptAvailList;
    QList<QString> nseScriptActiveList;
    QHash<QString, QTextDocument*> nseHelpCache;
    QHash<QString, QAction*> _collections;
    QHash<QString, QPushButtonOrientated*> _collectionsButton;
    QList<parserObj*> parserObjList;
    QList<parserObjUtil*> parserObjUtilList;
    QList<lookUpT*> internealLookupList;
    QList<digSupport*> digLookupList;
    QList<QWebView*> webViewList;
    QLabel *labelVersion;
    QLabel *userMode;
    QSplitter *cW;
    QSplitter *bW;
    QToolButton *menuSetup;
    monitor *_monitor;
    utilities *_utilities;

private slots:
    void initObject();
    void startScan();
    void stop_scan();
    void exit();
    void nmapParser(const QStringList parList, QByteArray dataBuffer,  QByteArray errorBuffer);
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
    void showStatusBar();
    void updateIconsBox();
    void callScanH();
    void callScanDiscover();
    void callParFromBook();
    void scanLookup(QHostInfo info, int state, const QString hostname);
    void runtimePars(QTreeWidgetItem *item, int column);
    void runtimeTraceroutePars(QTreeWidgetItem *item, int column);
    void monitorRuntimeEvent();
    void monitorStopCurrentScan();
    void monitorShowDetails();
    void updateScanCounter(int hostNumber);
    void objVulnButton();
    // Check nmap version
    void checkNmapVersion();
    //update and log slots
    void startProfile_ui();
    void readProfile();
    void saveAsLog();
    void saveLog();
    void updateFontHost();
    void updateFontHostVuln();
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
    void updateTabMonitor();
    void vulnPostScan();
    void updateComboVuln(const QString& value);
    void NSSCheck();
    void parAdv();
    void resetPar();
    void detailsOptions();
    void nssOptions();
    void bookOptions();
    void bookOptionsPar();
    // Vuln extension
    void searchVulnNG();
    void callSearchHistoryVuln();
    void callVulnCheck();
    void closeVulnTab(int index);
    void tabWebBack();
    void tabWebForward();
    void tabWebStop();
    void addUrlToBookmarks(const QString urlName, const QString urlAddr);
    void removeUrlToBookmarks();
    //contest Menu
    void menuScanBook();
    void menuVulnBook();
    void menuParBook();
    void menuServiceMain();
    void runtimeScanDiscover();
    void slotParSelected();
    void slotHostSelected();
    void saveUiSettings();
    // global profile
    void saveGlobalProfile();
    void removeGlobalProfile();
    void updateNseOptionScript(int index);
    void nseTreeDefaultValue();
    void nseTreeAvailRestoreValues();
    void nseTreeActiveRestoreValues();
    void nseTreeActiveItem();
    void nseTreeRemoveItem();
    void nseTreeResetItem();
    void startAddUrl_ui();
    void startAddParBook_ui();
    //discover
    void discoverIp(const QString& interface);
    void pingResult(QStringList hostname, bool state, const QByteArray callBuff);
    void discoverIpState();
    void discoveryClear();
    void stopDiscover();
    void updateSRdata();
    void requestNseHelp(QTreeWidgetItem *item, int column);
    void showNseHelp(const QStringList parameters, QByteArray result, QByteArray errors);
    void requestNseScriptHelp();
    void showNseScriptHelp(const QStringList parameters, QByteArray result, QByteArray errors);
};

#endif
