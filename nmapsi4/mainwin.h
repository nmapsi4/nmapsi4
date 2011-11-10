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

// local include
#include "core/preference/profilemain.h"
#include "../lib/history/loghistory.h"
#include "core/nmapsi4Debug.h"
#include "../lib/about/about.h"
#include "core/scanMT/scanThread.h"
#include "core/lookup/lookUpT.h"
#include "core/digSupport/digSupport.h"
#include "core/parserObj/parserObj.h"
#include "core/parserObj/parserObjUtil.h"
#include "core/vulnerability/addurl.h"
#include "core/discover/maindiscover.h"
#include "core/bookmarks/addparbook.h"
#include "core/lib/memorytools.h"
#include "core/monitor/details.h"
#include "core/monitor/monitor.h"

// Qt4 include
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>
#include <QtWebKit/QWebPage>
#include <QtCore/QHash>
#include <QtCore/QPointer>
#include <QtGui/QSplitter>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QtGui/QFileDialog>
#include <QtGui/QDesktopServices>
#include <QtGui/QToolButton>
#include <QtGui/QTextDocument>

// define class namespace
using namespace internalLookup;
using namespace scanning;
using namespace digInterface;
using namespace parserObject;
using namespace parserUtilObject;
using namespace pingInterface;
using namespace memory;

class QMainWindow;
class QWebView;

class nmapClass : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    nmapClass();
    ~nmapClass();

private:
    void scan(const QString hostname);
    void preScanLookup(const QString hostname);
    void rootMode();
    void show_browser(QLineEdit *location);
    void isEmptyLog();
    void setNmapsiSlot();
    QFile* create_logFile(const QString Path);
    QStringList check_extensions();
    void resetOptions();
    void checkProfile();
    void initGUI();
    bool isDns(QString hostname);
    QString clearHost(const QString hostname);
    bool checkViewOS(const QString OSline, QTreeWidgetItem *itemOS) const;
    void showParserObj(int indexObj);
    void showParserObjPlugins(int indexObj);
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
    void show_log_browserUrl(const QString url, QLineEdit *location);
    void setQuickProfile();
    void setNormalProfile();
    void setFullVersionProfile();
    void setQuickVersionProfile();

signals:
    void killDiscover();

protected:
    int uid;
    QFile *PFile;
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
    QHash<QString, scanThread*> scanHashList;
    QHash<QString, QStringList> scanHashListFlow;
    QHash<QString, QTextDocument*> nseHelpCache;
    QHash<QString, QAction*> _collections;
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

private slots:
    void initObject();
    void nmapParser(const QStringList parList, QByteArray Byte1,  QByteArray Byte2);
    void update_portCombo();
    void update_scanCombo();
    void update_inputcheck();
    void update_discover();
    void update_timing();
    void update_options();
    void update_comboVerbosity();
    void input_browser();
    void fileSession();
    void listClear();
    void checkFullScreen();
    void updateMenuBar();
    void showStatusBar();
    void show_bugUrl();
    void show_homepageUrl();
    void show_documentationUrl();
    void show_donateUrl();
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
    void setProgress();
    void readFlowFromThread(const QString hostname, QString lineData);
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
    void startScan();
    void about();
    void about_qt();
    void stop_scan();
    void exit();
};

#endif
