/*
Copyright 2007-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config-nmapsi4.h"

#include "ui_scanwidget.h"

// Qt4 include
#include <QMainWindow>
#include <QMessageBox>
#include <QSplitter>
#include <QCompleter>
#include <QStringListModel>
#include <QCloseEvent>
#include <QtCore/QHash>
#include <QtCore/QWeakPointer>
#include <QtCore/QTimer>
#include <QtCore/QList>
#include <QtCore/QFile>
#include <QtDeclarative/QDeclarativeContext>

// KDE include
#if defined(USE_KDELIBS)
#include <KMessageWidget>
#endif

// local include
#include "preferencesdialog.h"
#include "debug.h"
#include "addparameterstobookmark.h"
#include "memorytools.h"
#include "monitor.h"
#include "utilities.h"
#include "hostutilities.h"
#include "vulnerability.h"
#include "discovermanager.h"
#include "parsermanager.h"
#include "profilermanager.h"
#include "bookmarkmanager.h"
#include "actionmanager.h"
#include "notify.h"
#include "profilehandler.h"

// system
#if !defined(Q_OS_WIN32)
#include <unistd.h>
#endif

class ScanWidget : public QWidget, public Ui::ScanWidgetForm
{
   Q_OBJECT
public:
    explicit ScanWidget(QWidget* parent = 0);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void updateComboBook();
    void updateCompleter();
    void buildScanProfileList();

    Vulnerability* m_vulnerability;
    DiscoverManager* m_discoverManager;
    Monitor* m_monitor;
    Utilities* m_utilities;
    ParserManager* m_parser;
    BookmarkManager* m_bookmark;
    ActionManager* m_collections;
    ProfileHandler* m_profileHandler;
    int m_hostCache;
    ScanWidget* m_scanWidget;
    QDeclarativeView* m_welcomeQmlView;
    QTabWidget* m_mainTabWidget;

#if defined(USE_KDELIBS)
    KMessageWidget *m_kWidgetNotification;
#endif

private:
    void addHostToMonitor(const QString hostname);
    void resetOptions();
    void restoreSettings();
    void setDefaultSplitter();
    void updateQmlScanHistory();

    QSplitter *m_mainHorizontalSplitter;
    QSplitter *m_mainVerticalSplitter;
    QWeakPointer<QCompleter> m_completer;
    QWeakPointer<QStringListModel> m_hostModel;
    int m_userId;
    int m_lookupType;
    int m_savedProfileIndex;
    QByteArray m_scanListWidgetSize;
    QByteArray m_detailsWidgetSize;

protected:
    virtual void closeEvent(QCloseEvent * event);

public slots:
    void startScan();
    void updateComboHostnameProperties();
    void comboParametersSelectedEvent();
    Q_INVOKABLE void updateScanSection();
    Q_INVOKABLE void updateVulnerabilitySection();
    Q_INVOKABLE void updateDiscoverSection();

private slots:
    void initObject();
    void clearAll();
    void setFullScreen();
    void updateMenuBar();
    void takeHostFromBookmark();
    void updateScanCounter(int hostNumber);
    void startPreferencesDialog();
    void syncSettings();
    void linkCompleterToHostname();
    void updateWelcomeSection();
    void quickAddressSelectionEvent();
    void saveSettings();
    void newProfile();
    void editProfile();
    void clearHostnameCombo();
    void resizeVerticalSplitterEvent();
    void resizeHorizontalSplitterEvent();
    void resizeScanListWidgetEvent();
    void resizeHostDetailsWidgetEvent();
    void resetComboParameters();
    void loadTargetListFromFile();

#if defined(USE_KDELIBS)
    void hideKWidget();
#endif
};

#endif
