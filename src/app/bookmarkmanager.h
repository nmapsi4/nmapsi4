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

#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include "ui_scanbookmarkwidget.h"

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QSplitter>

#include "history.h"
#include "memorytools.h"

// system
#ifndef Q_WS_WIN
#include <unistd.h>
#endif

using namespace memory;

class MainWindow;

class ScanBookmarkWidget : public QWidget, public Ui::ScanBookmarkWidgetForm
{
    Q_OBJECT
public:
    explicit ScanBookmarkWidget(QWidget* parent = 0);
};

class BookmarkManager : public QObject
{
    Q_OBJECT

public:
    BookmarkManager(MainWindow* parent);
    ~BookmarkManager();
    /**
     * Restore all saved values on treeWidget
     */
    void restoreAllHistoryValues();
    /**
     * Save Vulnerability search address in bookmark
     */
    void saveAddressToBookmark(const QString addressName, const QString address);
    /**
     * Delete Vulnerability search address in bookmark
     */
    void deleteAddressFromBookmark(const QString addressName);
    /**
     * Save Host (ip or Dns or ip-range) to bookmark
     */
    void saveHostToBookmark(const QString hostName, int cacheLimit);
    /**
     * Save Vulnerability service to bookmark
     */
    void saveServiceToBookmark(const QString service, int cacheLimit);
    /**
     * Return stored host list from bookmark
     */
    QStringList getHostListFromBookmark();
    /**
     * Return stored Services list from bookmark
     */
    QStringList getServicesListFromBookmark();
    bool isBookmarkHostListEmpty();
    bool isBookmarkServicesListEmpty();
    void syncSettings();

    ScanBookmarkWidget* m_scanBookmarkWidget;

private:
    MainWindow* m_ui;
    int m_userId;
    QList<QTreeWidgetItem*> m_treeloghlist;
    QList<QTreeWidgetItem*> m_treebookparlist;
    QList<QTreeWidgetItem*> m_treebookvulnlist;
    QList<QTreeWidgetItem*> m_treewidgetvulnlist;
    QSplitter *m_scanBookmarkSplitter;
    QSplitter *m_vulnBookmarkSplitter;

public slots:
    /**
     * Delete selected Vulnerability service, host from bookmark (contextual menu)
     */
    void deleteItemFromBookmark();
    /**
     * Delete selected scan parameters to bookmark (contextual menu)
     */
    void deleteParametersFromBookmark();
    void startParametersToBookmarksDialog();

private slots:
    /**
     * Save selected Vulnerability service, host from bookmark (contextual menu)
     */
    void saveItemToBookmarks();
    /**
     * Save scan parameters to bookmark
     */
    void saveParametersToBookmarks(const QString profileName, const QString profileParameters);

};

#endif // BOOKMARKMANAGER_H
