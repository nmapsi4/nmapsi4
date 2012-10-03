/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QToolButton>
#include <QtGui/QToolBar>

#include <pushbuttonorientated.h>

class MainWindow;

class ActionManager : public QObject
{
    Q_OBJECT

public:
    ActionManager(MainWindow* parent = 0);
    ~ActionManager();
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

    QHash<QString, PushButtonOrientated*> m_collectionsButton;
    QHash<QString, QAction*> m_collectionsDiscover;
    QHash<QString, QAction*> m_collectionsScanSection;
    QHash<QString, QAction*> m_collectionsVulnerability;
    QToolBar *m_discoverToolBar;

private:
    MainWindow* m_ui;
    QToolButton *m_menuSetup;
    QToolButton *m_profilerTool;
    QToolButton *m_saveTool;
    QToolButton *m_bookmarksTool;
    QToolButton *m_discoverScanTool;
    QToolBar *m_bottomSectionBar;
    QToolBar *m_bottomUiToggleBar;
    QToolBar *m_scanToolBar;
    QToolBar *m_bookmarkToolBar;
    QToolBar *m_globalMenuToolBar;
    QToolBar *m_vulnerabilityToolBar;
    QMenu *m_menuBookmark;

public slots:
    void scanBookmarkContextMenu();
    void parametersBookmarkContextMenu();
    void mainServicesContextMenu();
    void servicesContextMenu();
    void vulnerabilityUrlContextMenu();

};

#endif // ACTIONMANAGER_H
