/***************************************************************************
 *   Copyright (C) 2007-2012 by Francesco Cecconi                          *
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
#include <QtGui/QSplitter>
#include <QtGui/QCompleter>
#include <QtGui/QStringListModel>
#include <QtGui/QCloseEvent>
#include <QtCore/QHash>
#include <QtCore/QWeakPointer>
#include <QtCore/QTimer>
#include <QtCore/QList>

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
#include "profilerdialog.h"
#include "bookmarkmanager.h"
#include "actionmanager.h"
#include "style.h"

// define class namespace
using namespace memory;

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow, public Ui::MainWindowClass
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent=0);
    ~MainWindow();
    void loadScanProfile();
    void updateComboBook();
    // NOTE: check completer section in vulnerability
    void updateCompleter();

    Vulnerability* m_vulnerability;
    DiscoverManager* m_discoverManager;
    Monitor* m_monitor;
    Utilities* m_utilities;
    ParserManager* m_parser;
    BookmarkManager* m_bookmark;
    ActionManager* m_collections;
    int m_hostCache;
    QWeakPointer<QCompleter> m_completerVuln;
    QWeakPointer<QStringListModel> m_vulnModel;

private:
    void addHostToMonitor(const QString hostname);
    void connectSlots();
    void resetOptions();
    void initGUI();
    void setTreeWidgetValues();
    void restoreSettings();
    void setTreeSettings();
    void setDefaultAction();
    void setDefaultSplitter();
    void defaultComboValues();
    QStringList loadExtensions();
    QHash<QString, QString> defaultScanProfile() const;

    QSplitter *m_mainHorizontalSplitter;
    QSplitter *m_mainVerticalSplitter;
    QWeakPointer<QCompleter> m_completer;
    QWeakPointer<QStringListModel> m_hostModel;
    int m_userId;
    int m_lookupType;
    bool m_savePos;
    bool m_saveSize;
    QByteArray m_scanListWidgetSize;
    QByteArray m_detailsWidgetSize;

protected:
    /**
     * Reimplement closeEvent protected function
     */
    virtual void closeEvent(QCloseEvent * event);

public slots:
    void startScan();
    void updateComboHostnameProperties();

private slots:
    void initObject();
    void clearAll();
    void checkFullScreen();
    void updateMenuBar();
    void updateIconsBox();
    void takeHostFromBookmark();
    void monitorRuntimeEvent();
    void updateScanCounter(int hostNumber);
    void startPreferencesDialog();
    void syncSettings();
    void linkCompleterToHostname();
    void updateSezScan();
    void updateSezVuln();
    void updateSezDiscover();
    void loadDefaultProfile();
    void resetComboParameters();
    void detailsOptions();
    void nssOptions();
    void comboParametersSelectedEvent();
    void quickAddressSelectionEvent();
    void saveSettings();
    void newProfile();
    void editProfile();
    void clearHostnameCombo();
    void clearParametersCombo();
    void resizeVerticalSplitterEvent();
    void resizeHorizontalSplitterEvent();
    void resizeScanListWidgetEvent();
    void resizeHostDetailsWidgetEvent();

};

#endif