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

#include "bookmarkmanager.h"
#include "mainwindow.h"

ScanBookmarkWidget::ScanBookmarkWidget(QWidget* parent): QWidget(parent)
{
    setupUi(this);
}

VulnBookmarkWidget::VulnBookmarkWidget(QWidget* parent): QWidget(parent)
{
    setupUi(this);
}

BookmarkManager::BookmarkManager(MainWindow* parent)
    : QObject(parent), m_ui(parent), m_userId(0)
{
#if !defined(Q_OS_WIN32)
    m_userId = getuid();
#endif

    m_scanBookmarkWidget = new ScanBookmarkWidget(m_ui);
    m_vulnBookmarkWidget = new VulnBookmarkWidget(m_ui);

    m_scanBookmarkWidget->treeLogH->setColumnWidth(0, 400);
    m_scanBookmarkWidget->treeBookPar->setColumnWidth(0, 400);
    m_vulnBookmarkWidget->treeBookVuln->setColumnWidth(0, 400);
    m_vulnBookmarkWidget->treeWidgetVulnUrl->setColumnWidth(0, 400);

    QSettings settings("nmapsi4", "nmapsi4");

    m_scanBookmarkSplitter = new QSplitter(m_ui);
    m_scanBookmarkSplitter->setOrientation(Qt::Vertical);
    m_scanBookmarkSplitter->addWidget(m_scanBookmarkWidget->treeLogH);
    m_scanBookmarkSplitter->addWidget(m_scanBookmarkWidget->treeBookPar);

    m_vulnBookmarkSplitter = new QSplitter(m_ui);
    m_vulnBookmarkSplitter->setOrientation(Qt::Vertical);
    m_vulnBookmarkSplitter->addWidget(m_vulnBookmarkWidget->treeBookVuln);
    m_vulnBookmarkSplitter->addWidget(m_vulnBookmarkWidget->treeWidgetVulnUrl);

    m_scanBookmarkWidget->layout()->addWidget(m_scanBookmarkSplitter);
    m_vulnBookmarkWidget->layout()->addWidget(m_vulnBookmarkSplitter);

    if (!settings.value("scanBookmarkSplitter").toByteArray().isEmpty()) {
        m_scanBookmarkSplitter->restoreState(settings.value("scanBookmarkSplitter").toByteArray());
    }

    if (!settings.value("vulnBookmarkSplitter").toByteArray().isEmpty()) {
        m_vulnBookmarkSplitter->restoreState(settings.value("vulnBookmarkSplitter").toByteArray());
    }
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

void BookmarkManager::saveHostnameItemToBookmark()
{
    if (m_ui->m_scanWidget->hostEdit->currentText().isEmpty()) {
        return;
    }

    saveItemToBookmarks(m_ui->m_scanWidget->hostEdit->currentText());
}

void BookmarkManager::saveServiceItemToBookmark()
{
    if (m_ui->m_vulnerability->m_vulnerabilityWidget->comboVulnRis->currentText().isEmpty()) {
        return;
    }

    saveItemToBookmarks(m_ui->m_vulnerability->m_vulnerabilityWidget->comboVulnRis->currentText());
}

void BookmarkManager::saveItemToBookmarks(QString value)
{
    if (m_ui->m_collections->m_collectionsButton.value("scan-sez")->isChecked()) {
        // save list of ip/dns in input
        History *history_ = new History(m_scanBookmarkWidget->treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);

        // save list of address
        QStringList hostList = value.split(' ', QString::KeepEmptyParts);

        foreach(const QString & host, hostList) {
            history_->addItemHistory(host, QDateTime::currentDateTime().toString("MMMM d yyyy - hh:mm:ss"));
        }

        memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeloghlist);
        m_treeloghlist = history_->updateBookMarks();

        delete history_;
        m_ui->updateComboBook();
        m_ui->m_scanWidget->hostEdit->lineEdit()->setText(value);
    } else {
        History *history_ = new History(m_vulnBookmarkWidget->treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
        history_->addItemHistory(value,QDateTime::currentDateTime().toString("MMMM d yyyy - hh:mm:ss"));

        memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookvulnlist);
        m_treebookvulnlist = history_->updateBookMarks();

        delete history_;
        m_ui->updateComboBook();
        m_ui->m_vulnerability->m_vulnerabilityWidget->comboVulnRis->lineEdit()->setText(value);
    }
}

void BookmarkManager::deleteItemFromBookmark()
{
    if (!m_scanBookmarkWidget->treeLogH->currentItem() && !m_vulnBookmarkWidget->treeBookVuln->currentItem()) {
        return;
    }

    History *history_;

    if (m_ui->m_collections->m_collectionsButton.value("scan-sez")->isChecked()) {
        history_ = new History(m_scanBookmarkWidget->treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
        history_->deleteItemBookmark(m_scanBookmarkWidget->treeLogH->currentItem()->text(0));

        memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeloghlist);
        m_treeloghlist = history_->updateBookMarks();
    } else {
        history_ = new History(m_vulnBookmarkWidget->treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
        history_->deleteItemBookmark(m_vulnBookmarkWidget->treeBookVuln->currentItem()->text(0));

        memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookvulnlist);
        m_treebookvulnlist = history_->updateBookMarks();
    }

    delete history_;
    m_ui->updateComboBook();
}

void BookmarkManager::deleteParametersFromBookmark()
{

    if (!m_scanBookmarkWidget->treeBookPar->currentItem()) {
        return;
    }

    History *history_;

    if (!m_userId) {
        history_ = new History(m_scanBookmarkWidget->treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
        history_->deleteItemBookmark(m_scanBookmarkWidget->treeBookPar->currentItem()->text(0));
    } else {
        history_ = new History(m_scanBookmarkWidget->treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
        history_->deleteItemBookmark(m_scanBookmarkWidget->treeBookPar->currentItem()->text(0));
    }

    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookparlist);
    m_treebookparlist = history_->updateBookMarks();

    delete history_;
    m_ui->buildScanProfileList();
}

void BookmarkManager::startParametersToBookmarksDialog()
{
    const QString& parameters = m_ui->m_scanWidget->comboAdv->currentText();

    if (parameters.isEmpty()) {
        return;
    }

    QWeakPointer<addParametersToBookmark> dialogParAdd = new addParametersToBookmark(m_ui, parameters);

    connect(dialogParAdd.data(), SIGNAL(doneParBook(QString,QString)),
            this, SLOT(saveParametersToBookmarks(QString,QString)));

    dialogParAdd.data()->exec();

    if (!dialogParAdd.isNull()) {
        delete dialogParAdd.data();
    }
}

void BookmarkManager::saveParametersToBookmarks(const QString profileName, const QString profileParameters)
{
    if (m_ui->m_scanWidget->comboAdv->currentText().isEmpty()) {
        return;
    }

    History *history_;

    if (!m_userId) {
        history_ = new History(m_scanBookmarkWidget->treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);

        if (!history_->isProfileInHistory(profileName)) {
            history_->addItemHistory(profileParameters, profileName);
        } else {
            history_->updateProfile(profileParameters, profileName);
        }
    } else {
        history_ = new History(m_scanBookmarkWidget->treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);

        if (!history_->isProfileInHistory(profileName)) {
            history_->addItemHistory(profileParameters, profileName);
        } else {
            history_->updateProfile(profileParameters, profileName);
        }
    }

    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookparlist);
    m_treebookparlist = history_->updateBookMarks();

    delete history_;
    m_ui->buildScanProfileList();
}

void BookmarkManager::restoreAllHistoryValues()
{
    History *newHistory;
    // reload bookmark from history
    newHistory = new History(m_scanBookmarkWidget->treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
    m_treeloghlist = newHistory->updateBookMarks();
    delete newHistory;
    // check for user or admin parameters bookmarks
    if (!m_userId) {
        newHistory = new History(m_scanBookmarkWidget->treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
        m_treebookparlist = newHistory->updateBookMarks();
        delete newHistory;
    } else {
        newHistory = new History(m_scanBookmarkWidget->treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
        m_treebookparlist = newHistory->updateBookMarks();
        delete newHistory;
    }

    newHistory = new History(m_vulnBookmarkWidget->treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
    m_treebookvulnlist = newHistory->updateBookMarks();
    delete newHistory;

    newHistory = new History(m_vulnBookmarkWidget->treeWidgetVulnUrl, "nmapsi4/nameUrlVuln", "nmapsi4/nameUrlAddr", -1);
    m_treewidgetvulnlist = newHistory->updateBookMarks();
    delete newHistory;
}

void BookmarkManager::saveAddressToBookmark(const QString addressName, const QString address)
{
    History *newHistory = new History(m_vulnBookmarkWidget->treeWidgetVulnUrl, "nmapsi4/nameUrlVuln" , "nmapsi4/nameUrlAddr", -1);
    newHistory->addItemHistory(addressName, address);

    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treewidgetvulnlist);
    m_treewidgetvulnlist = newHistory->updateBookMarks();
    delete newHistory;
}

void BookmarkManager::deleteAddressFromBookmark(const QString addressName)
{
    History *newHistory = new History(m_vulnBookmarkWidget->treeWidgetVulnUrl, "nmapsi4/nameUrlVuln" , "nmapsi4/nameUrlAddr", -1);
    newHistory->deleteItemBookmark(addressName);

    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treewidgetvulnlist);
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
    if (getHostListFromBookmark().isEmpty()) {
        return true;
    }

    return false;
}

bool BookmarkManager::isBookmarkServicesListEmpty()
{
    if (getServicesListFromBookmark().isEmpty()) {
        return true;
    }

    return false;
}
