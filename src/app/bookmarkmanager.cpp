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

#include "bookmarkmanager.h"
#include "mainwindow.h"

BookmarkManager::BookmarkManager(MainWindow* parent)
: QObject(parent), m_ui(parent), m_userId(0)
{

#ifndef Q_WS_WIN
    m_userId = getuid();
#endif

    QSettings settings("nmapsi4", "nmapsi4");

    m_scanBookmarkSplitter = new QSplitter(m_ui);
    m_scanBookmarkSplitter->setOrientation(Qt::Vertical);
    m_scanBookmarkSplitter->addWidget(m_ui->treeLogH);
    m_scanBookmarkSplitter->addWidget(m_ui->treeBookPar);

    m_vulnBookmarkSplitter = new QSplitter(m_ui);
    m_vulnBookmarkSplitter->setOrientation(Qt::Vertical);
    m_vulnBookmarkSplitter->addWidget(m_ui->treeBookVuln);
    m_vulnBookmarkSplitter->addWidget(m_ui->treeWidgetVulnUrl);

    m_ui->tabUi->widget(m_ui->tabUi->indexOf(m_ui->bookmarks))->layout()->addWidget(m_scanBookmarkSplitter);
    m_ui->tabUi->widget(m_ui->tabUi->indexOf(m_ui->tabVulnBookmarks))->layout()->addWidget(m_vulnBookmarkSplitter);

    if (!settings.value("scanBookmarkSplitter").toByteArray().isEmpty())
    {
        m_scanBookmarkSplitter->restoreState(settings.value("scanBookmarkSplitter").toByteArray());
    }

    if (!settings.value("vulnBookmarkSplitter").toByteArray().isEmpty())
    {
        m_vulnBookmarkSplitter->restoreState(settings.value("vulnBookmarkSplitter").toByteArray());
    }

    connect(m_ui->actionAdd_Bookmark, SIGNAL(triggered()),
            this, SLOT(saveItemToBookmarks()));
    connect(m_ui->actionAdd_Parameters_to_bookmark, SIGNAL(triggered()),
            this, SLOT(startParametersToBookmarksDialog()));

}

BookmarkManager::~BookmarkManager()
{
}

void BookmarkManager::syncSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("scanBookmarkSplitter", m_scanBookmarkSplitter->saveState());
    settings.setValue("vulnBookmarkSplitter", m_vulnBookmarkSplitter->saveState());
}

void BookmarkManager::saveItemToBookmarks()
{
    if(m_ui->hostEdit->currentText().isEmpty() && m_ui->comboVulnRis->currentText().isEmpty())
    {
        return;
    }

    if (m_ui->m_collections->m_collectionsButton.value("scan-sez")->isChecked())
    {
        const QString& currentHost = m_ui->hostEdit->currentText();
        // save list of ip/dns in input
        History *history_ = new History(m_ui->treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);

        QStringList hostList = m_ui->hostEdit->currentText().split(' ', QString::KeepEmptyParts);

        foreach (const QString& host, hostList)
        {
            history_->addItemHistory(host, QDateTime::currentDateTime().toString("MMMM d yyyy - hh:mm:ss"));
        }

        freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeloghlist);
        m_treeloghlist = history_->updateBookMarks();

        delete history_;
        m_ui->updateComboBook();
        m_ui->hostEdit->lineEdit()->setText(currentHost);
    }
    else
    {
        const QString& currentHost = m_ui->comboVulnRis->currentText();
        History *history_ = new History(m_ui->treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
        history_->addItemHistory(m_ui->comboVulnRis->currentText(),
                                 QDateTime::currentDateTime().toString("MMMM d yyyy - hh:mm:ss"));

        freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookvulnlist);
        m_treebookvulnlist = history_->updateBookMarks();

        delete history_;
        m_ui->updateComboBook();
        m_ui->comboVulnRis->lineEdit()->setText(currentHost);
    }
}

void BookmarkManager::deleteItemFromBookmark()
{
    if(!m_ui->treeLogH->currentItem() && !m_ui->treeBookVuln->currentItem())
    {
        return;
    }

    History *history_;

    if (m_ui->m_collections->m_collectionsButton.value("scan-sez")->isChecked())
    {
        history_ = new History(m_ui->treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
        history_->deleteItemBookmark(m_ui->treeLogH->currentItem()->text(0));

        freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeloghlist);
        m_treeloghlist = history_->updateBookMarks();
    }
    else
    {
        history_ = new History(m_ui->treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
        history_->deleteItemBookmark(m_ui->treeBookVuln->currentItem()->text(0));

        freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookvulnlist);
        m_treebookvulnlist = history_->updateBookMarks();
    }

    delete history_;
    m_ui->updateComboBook();
}

void BookmarkManager::deleteParametersFromBookmark()
{

    if(!m_ui->treeBookPar->currentItem())
    {
        return;
    }

    History *history_;

    if (!m_userId)
    {
        history_ = new History(m_ui->treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
        history_->deleteItemBookmark(m_ui->treeBookPar->currentItem()->text(0));
    }
    else
    {
        history_ = new History(m_ui->treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
        history_->deleteItemBookmark(m_ui->treeBookPar->currentItem()->text(0));
    }

    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookparlist);
    m_treebookparlist = history_->updateBookMarks();

    delete history_;
    m_ui->loadScanProfile();
}

void BookmarkManager::startParametersToBookmarksDialog()
{
    const QString& parameters = m_ui->comboAdv->currentText();

    if (parameters.isEmpty())
    {
        return;
    }

    QWeakPointer<addParametersToBookmark> dialogParAdd = new addParametersToBookmark(m_ui, parameters);

    connect(dialogParAdd.data(), SIGNAL(doneParBook(QString,QString)),
            this, SLOT(saveParametersToBookmarks(QString,QString)));

    dialogParAdd.data()->exec();

    if (!dialogParAdd.isNull())
    {
        delete dialogParAdd.data();
    }
}

void BookmarkManager::saveParametersToBookmarks(const QString profileName, const QString profileParameters)
{
    if(m_ui->comboAdv->currentText().isEmpty())
    {
        return;
    }

    History *history_;

    if (!m_userId)
    {
        history_ = new History(m_ui->treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);

        if (!history_->isProfileInHistory(profileName))
        {
            history_->addItemHistory(profileParameters, profileName);
        }
        else
        {
            history_->updateProfile(profileParameters,profileName);
        }
    }
    else
    {
        history_ = new History(m_ui->treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);

        if (!history_->isProfileInHistory(profileName))
        {
            history_->addItemHistory(profileParameters, profileName);
        }
        else
        {
            history_->updateProfile(profileParameters,profileName);
        }
    }

    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookparlist);
    m_treebookparlist = history_->updateBookMarks();

    delete history_;
    m_ui->loadScanProfile();
}

void BookmarkManager::restoreAllHistoryValues()
{
    History *newHistory;
    // reload bookmark from history
    newHistory = new History(m_ui->treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
    m_treeloghlist = newHistory->updateBookMarks();
    delete newHistory;
    // check for user or admin parameters bookmarks
    if (!m_userId)
    {
        newHistory = new History(m_ui->treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
        m_treebookparlist = newHistory->updateBookMarks();
        delete newHistory;
    }
    else
    {
        newHistory = new History(m_ui->treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
        m_treebookparlist = newHistory->updateBookMarks();
        delete newHistory;
    }

    newHistory = new History(m_ui->treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
    m_treebookvulnlist = newHistory->updateBookMarks();
    delete newHistory;

    newHistory = new History(m_ui->treeWidgetVulnUrl, "nmapsi4/nameUrlVuln", "nmapsi4/nameUrlAddr", -1);
    m_treewidgetvulnlist = newHistory->updateBookMarks();
    delete newHistory;
}

void BookmarkManager::saveAddressToBookmark(const QString addressName, const QString address)
{
    History *newHistory = new History(m_ui->treeWidgetVulnUrl, "nmapsi4/nameUrlVuln" , "nmapsi4/nameUrlAddr", -1);
    newHistory->addItemHistory(addressName, address);

    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treewidgetvulnlist);
    m_treewidgetvulnlist = newHistory->updateBookMarks();
    delete newHistory;
}

void BookmarkManager::deleteAddressFromBookmark(const QString addressName)
{
    History *newHistory = new History(m_ui->treeWidgetVulnUrl, "nmapsi4/nameUrlVuln" , "nmapsi4/nameUrlAddr", -1);
    newHistory->deleteItemBookmark(addressName);

    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treewidgetvulnlist);
    m_treewidgetvulnlist = newHistory->updateBookMarks();
    delete newHistory;
}

void BookmarkManager::saveHostToBookmark(const QString hostName, int cacheLimit)
{
    History *newHistory = new History("nmapsi4/cacheHost", cacheLimit);
    newHistory->addItemHistory(hostName);
    delete newHistory;
}

void BookmarkManager::saveServiceToBookmark(const QString service, int cacheLimit)
{
    History *newHistory = new History("nmapsi4/cacheVuln", cacheLimit);
    newHistory->addItemHistory(service);
    delete newHistory;
}

QStringList BookmarkManager::getHostListFromBookmark()
{
    History *newHistory = new History("nmapsi4/cacheHost");
    QStringList hostCache = newHistory->getHostCache();
    delete newHistory;

    return hostCache;
}

QStringList BookmarkManager::getServicesListFromBookmark()
{
    History *newHistory = new History("nmapsi4/cacheVuln");
    QStringList hostCache = newHistory->getHostCache();
    delete newHistory;

    return hostCache;
}

bool BookmarkManager::isBookmarkHostListEmpty()
{
    if (getHostListFromBookmark().first() != "NULL")
    {
        return false;
    }

    return true;
}

bool BookmarkManager::isBookmarkServicesListEmpty()
{
    if (getServicesListFromBookmark().first() != "NULL")
    {
        return false;
    }

    return true;
}
