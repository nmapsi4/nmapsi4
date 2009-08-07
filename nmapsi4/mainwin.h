/***************************************************************************
 *   Copyright (C) 2007-2009 by Francesco Cecconi                          *
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
// FIXME
#include "core/digSupport/digSupport.h"
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>

class QMainWindow;
class QWebView;
class QMutex;

class nmapClass : public QMainWindow , private Ui::MainWindow
{
    Q_OBJECT

private:
    void scan(const QString hostname);
    void rootMode(int uid);
    void show_browser(QLineEdit *location);
    void isEmptyLog();
    void setNmapsiSlot();
    QFile* create_logFile(const QString Path);
    QStringList check_extensions(QStringList parametri, QString& winTitle);
    void resetOptions();
    void checkProfile();
    void init();
    void itemDeleteAll(QList<QTreeWidgetItem*> items);
    void addMonitorHost(QTreeWidget* monitor, const QString host);
    void delMonitorHost(QTreeWidget* monitor, const QString host);
    void updateMonitorHost(QTreeWidget* monitor);
    bool isDns(QString hostname);

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
    QTreeWidgetItem *root, *item2, *infoItem, *infoItemObj;
    QTreeWidgetItem *root2, *item_root2;
    QTreeWidgetItem *error, *item_error;
    QListWidgetItem *scanW, *toolW, *discoverW, *fileW, * timingW;
    QTreeWidgetItem *rootLook, *itemLook;
    QFile *PFile;
    QString FileName;
    QString firstPath;
    QString Profile;
    QString logPath;
    bool checkLog;
    bool flag_state;
    bool savePos;
    bool saveSize;
    bool listClearFlag;
    bool verboseLog;
    bool lookupInternal_;
    bool lookupDig_;
    int hostCache;
    QList<QTreeWidgetItem*> itemList;
    QList<QTreeWidgetItem*> itemListLook;
    QString logSessionFile;
    QLabel *labelVersion;
    QLabel *userMode;
    mainProfile *dialog;
    logHistory *history;
    scanThread *th;
    QList<QString> monitorElemHost;
    QList<QString> monitorElemState;
    QList<QTreeWidgetItem*> monitorElem;
    digSupport *digC;


public slots:
    void startScan();
    void about();
    void about_qt();
    void stop_scan();
    void exit();


private slots:
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
    void scanLookup(QHostInfo info, int state, const QString hostname);

    // Check nmap version
    void checkNmapVersion();
    void setNmapVersion();

    //update and log slots
    void startProfile_ui();
    void readProfile();
    void saveAsLog();
    void saveLog();
    void updateFontHost(const QString hostName);
    void callSearchHistory();
    void saveBookMarks();
    void deleteBookMark();
    void activeBookTool();
    void optionListCreate();
    void optionListUpdate();
    void setProgress();
    void updateMainSlide();

    // Vuln extension
    void searchVuln();
    void callSearchHistoryVuln();
    void callVulnCheck();
};

#endif
