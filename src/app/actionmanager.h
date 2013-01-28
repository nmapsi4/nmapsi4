/*
Copyright 2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <QToolBar>
#include <QMenuBar>

class MainWindow;

class ActionManager : public QObject
{
    Q_OBJECT

public:
    explicit ActionManager(MainWindow* parent = 0);
    ~ActionManager() {}
    void createToolButtonBar();
    void createSectionsBar();
    void createDiscoverBar();
    void createScanSectionBar();
    void createVulnerabilityBar();
    void enableBottomUiToggleActions();
    void disableBottomUiToggleActions();
    void enableScanSectionToolBar();
    void disableScanSectionToolBar();
    void enableBookmarkToolBar();
    void disableBookmarkToolBar();
    void enableGlobalMenuToolBar();
    void disableGlobalMenuToolBar();
    void enableVulnerabilityToolBar();
    void disableVulnerabilityToolBar();
    void enableScanBookmarkMenu();
    void enableVulnerabilityBookmarkMenu();
    void disableBookmarkMenu();
    void enableSaveActions();
    void disableSaveActions();

    QHash<QString, QToolButton*> m_collectionsButton;
    QHash<QString, QAction*> m_collectionsDiscover;
    QHash<QString, QAction*> m_collectionsScanSection;
    QHash<QString, QAction*> m_collectionsVulnerability;
    QToolBar *m_discoverToolBar;

private:
    void setupActions();
    void setupMenuBar();

    MainWindow* m_ui;
    QToolButton *m_menuSetup;
    QToolButton *m_profilerTool;
    QToolButton *m_saveTool;
    QToolButton *m_bookmarksTool;
    QToolButton *m_discoverScanTool;
    QToolBar *m_rightSectionBar;
    QToolBar *m_bottomUiToggleBar;
    QToolBar *m_scanToolBar;
    QToolBar *m_bookmarkToolBar;
    QToolBar *m_globalMenuToolBar;
    QToolBar *m_vulnerabilityToolBar;
    QMenu *m_menuBookmark;
    QMenu *m_menuFile;
    QMenu *m_menuSettings;
    QMenu *m_menuTools;
    QMenu *m_menuHelp;

private slots:
    void mouseClickEvent(QObject* obj);

public slots:
    void scanBookmarkContextMenu();
    void scanHostInfoContextMenu();
    void scanPortsInfoContextMenu();
    void scanFullOutputContextMenu();
    void parametersBookmarkContextMenu();
    void servicesContextMenu();
    void vulnerabilityUrlContextMenu();

};

#endif // ACTIONMANAGER_H
