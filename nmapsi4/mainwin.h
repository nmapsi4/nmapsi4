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
#include "preference/profilemain.h"
#include "../lib/history/loghistory.h"
#include "core/nmapsi4Debug.h"
#include "../lib/about/about.h"
#include "core/scanMT/scanThread.h"
#include "core/lookup/lookUpT.h"
#include "core/digSupport/digSupport.h"
#include "core/info/parserObj.h"
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>
#include <QtWebKit/QWebPage>


class QMainWindow;
class QWebView;
class QMutex;

class nmapClass : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

private:
    void scan(const QString hostname);
    void rootMode(int uid);
    void show_browser(QLineEdit *location);
    void isEmptyLog();
    void setNmapsiSlot();
    QFile* create_logFile(const QString Path);
    QStringList check_extensions();
    void resetOptions();
    void checkProfile();
    void initGUI();
    void itemDeleteAll(QList<QTreeWidgetItem*> items);
    void addMonitorHost(QTreeWidget* monitor, const QString host);
    void delMonitorHost(QTreeWidget* monitor, const QString host);
    void updateMonitorHost(QTreeWidget* monitor);
    void updateScanCounter(int type);
    bool isDns(QString hostname);
    QString clearHost(const QString hostname);
    void checkViewOS(const QString OSline, QTreeWidgetItem *itemOS);
    void showParserObj(int indexObj);
    void showParserObjPlugins(int indexObj);
    void setTreeWidgetValues();
    void createBar();
    void updateComboPar();


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
    QProcess *versionProc;
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
    bool HostDetEnabled;
    bool MonitorEnabled;
    bool TraceEnabled;
    bool LookupEnabled;
    int hostCache;
    int scanCounter;
    QList<QTreeWidgetItem*> itemList;
    QList<QTreeWidgetItem*> itemListScan;
    QList<QTreeWidgetItem*> itemListLook;
    QList<QTreeWidgetItem*> monitorElem;
    QList<QTreeWidgetItem*> objElem;
    QList<QTreeWidgetItem*> mainTreeElem;
    QList<QString> monitorElemHost;
    QList<QString> monitorElemState;
    QList<QString> monitorElemOptions;
    QList<scanThread*> scanPointerList;
    QList<parserObj*> parserObjList;
    QList<QWebView*> webViewList;
    QLabel *labelVersion;
    QLabel *userMode;
    QPointer<scanThread> th;
    QPointer<mainProfile> dialog;
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
    QAction *actTabHost;
    QAction *actTabMonitor;
    lookUpT *lth;
    digSupport *digC;
    logHistory *history;
    QSplitter *cW;
    QSplitter *rW;


public slots:
    void startScan();
    void about();
    void about_qt();
    void stop_scan();
    void exit();


private slots:
    void initObject();
    void nmapParser(const QString hostCheck, QByteArray Byte1,  QByteArray Byte2);
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
    void closeTree();
    void checkFullScreen();
    void updateMenuBar();
    void showMainToolBar();
    void showActionToolBar();
    void showStatusBar();
    void updateIconsBox();
    void callScanH();
    void callParFromBook();
    void scanLookup(QHostInfo info, int state, const QString hostname);
    void runtimePars(QTreeWidgetItem *item, int column);
    void runtimeTraceroutePars(QTreeWidgetItem *item, int column);
    void showObjServData(QTreeWidgetItem *item, int column);
    void objVulnButton();

    // Check nmap version
    void checkNmapVersion();
    void setNmapVersion();

    //update and log slots
    void startProfile_ui();
    void readProfile();
    void saveAsLog();
    void saveLog();
    void updateFontHost(const QString hostName);
    void updateFontHostVuln(const QString hostName);
    void callSearchHistory();
    void saveBookMarks();
    void saveBookMarksPar();
    void deleteBookMark();
    void deleteBookMarkPar();
    void optionListCreate();
    void optionListUpdate();
    void setProgress();

    void updateSezScan();
    void updateSezLog();
    void updateSezVuln();
    void updateTabLook();
    void updateTabTrace();
    void updateTabHost();
    void updateTabMonitor();
    void vulnPostScan();
    void updateComboVuln(const QString& value);
    void NSSCheck();
    void parAdv();
    void resetPar();
    void detailsOptions();
    void nssOptions();
    void infoOptions();
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

    //contest Menu
    void menuScanBook();
    void menuVulnBook();
    void menuParBook();
    void menuServiceMain();
    void slotParSelected();
    void saveUiSettings();
    void actDetails();
};

#endif
