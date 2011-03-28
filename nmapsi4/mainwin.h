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
#include "preference/profilemain.h"
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

// Qt4 include
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>
#include <QtWebKit/QWebPage>
#include <QMap>
#include <QPointer>
#include <QSplitter>
#include <QTimer>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>

class QMainWindow;
class QWebView;
class QMutex;

class nmapClass : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

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
    void itemDeleteAll(QList<QTreeWidgetItem*>& items);
    void itemDeleteAll(QList<parserObj*>& items);
    void itemDeleteAll(QList<parserObjUtil*>& items);
    void itemDeleteAll(QList<QWebView*>& items);
    void itemDeleteAll(QList<scanThread*>& items);
    void itemDeleteAll(QList<lookUpT*>& items);
    void itemDeleteAll(QList<digSupport*>& items);
    void addMonitorHost(QTreeWidget* monitor, const QString host);
    void delMonitorHost(QTreeWidget* monitor, const QString host);
    void updateMonitorHost(QTreeWidget* monitor);
    void updateScanCounter(int type);
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
    QMap<QString, QString> defaultScanProfile() const;
    QMap<QString, QString> defaultUrlVuln() const;
    void startDiscover();

public:
    nmapClass();
    ~nmapClass();
    void show_log_browserUrl(const QString url, QLineEdit *location);
    void setQuickProfile();
    void setNormalProfile();
    void setFullVersionProfile();
    void setQuickVersionProfile();

signals:
    void killScan();

protected:
    int uid;
    QProcess *versionProc;
    QFile *PFile;
    QListWidgetItem *scanW;
    QListWidgetItem *toolW;
    QListWidgetItem *discoverW;
    QListWidgetItem *fileW;
    QListWidgetItem *timingW;
    QListWidgetItem *nseW;
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
    int scanCounter;
    QList<QTreeWidgetItem*> itemListScan;
    QList<QTreeWidgetItem*> itemNseAvail;
    QList<QTreeWidgetItem*> itemNseActive;
    QList<QTreeWidgetItem*> monitorElem;
    QList<QTreeWidgetItem*> objElem;
    QList<QTreeWidgetItem*> mainTreeElem;
    QList<QString> monitorElemHost;
    QList<QString> monitorElemState;
    QList<QString> monitorElemOptions;
    QList<QString> nseScriptAvailList;
    QList<QString> nseScriptActiveList;
    QList<scanThread*> scanPointerList;
    QList<parserObj*> parserObjList;
    QList<parserObjUtil*> parserObjUtilList;
    QList<lookUpT*> internealLookupList;
    QList<digSupport*> digLookupList;
    QList<QWebView*> webViewList;
    QLabel *labelVersion;
    QLabel *userMode;
    QPointer<mainProfile> dialog;
    QPointer<mainUrlClass> m_dialogUrl;
    QAction *scanSez;
    QAction *logSez;
    QAction *vulnSez;
    QAction *nssAct;
    QAction *parAct;
    QAction *actSearch;
    QAction *actBack;
    QAction *actForward;
    QAction *actStop;
    QAction *actTabLook;
    QAction *actTabTrace;
    QAction *actTabMonitor;
    QSplitter *cW;
    QSplitter *bW;

public slots:
    void startScan();
    void about();
    void about_qt();
    void stop_scan();
    void exit();

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
    void showMainToolBar();
    void showActionToolBar();
    void showStatusBar();
    void show_bugUrl();
    void show_homepageUrl();
    void show_documentationUrl();
    void show_donateUrl();
    void updateIconsBox();
    void callScanH();
    void callParFromBook();
    void scanLookup(QHostInfo info, int state, const QString hostname);
    void runtimePars(QTreeWidgetItem *item, int column);
    void runtimeTraceroutePars(QTreeWidgetItem *item, int column);
    void objVulnButton();
    // Check nmap version
    void checkNmapVersion();
    void setNmapVersion();
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
    void readFlowFromThread(const QString hostname, const QString lineData);
    void updateSezScan();
    void updateSezLog();
    void updateSezVuln();
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
    void menuUrlBook();
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
    void discoverIp(mainDiscover *discover, QNetworkInterface interface);
};

#endif
