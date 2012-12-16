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

#include "mainwindow.h"

void MainWindow::setFullScreen()
{
    if (isFullScreen()) {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
        m_collections->m_collectionsScanSection.value("fullscreen-action")->setChecked(false);
    } else {
        setWindowState(windowState() | Qt::WindowFullScreen);
        m_collections->m_collectionsScanSection.value("fullscreen-action")->setChecked(true);
    }
}

void MainWindow::updateMenuBar()
{
    if (m_collections->m_collectionsScanSection.value("showmenubar-action")->isChecked()) {
        menuBar()->setVisible(true);
        m_collections->disableGlobalMenuToolBar();
    } else {
        menuBar()->setVisible(false);
        m_collections->enableGlobalMenuToolBar();
    }
}

void MainWindow::updateComboHostnameProperties()
{
    m_collections->m_collectionsScanSection.value("scan-action")->setEnabled(true);
    m_scanWidget->hostEdit->clear();
    m_scanWidget->hostEdit->setStyleSheet(QString::fromUtf8(""));
    bool signalState = m_scanWidget->hostEdit->lineEdit()->disconnect(SIGNAL(cursorPositionChanged(int,int)));

    if (!signalState) {
        return;
    }

    connect(m_scanWidget->hostEdit, SIGNAL(editTextChanged(QString)),
            this, SLOT(linkCompleterToHostname()));
}

void MainWindow::updateScanSection()
{
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(true);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(false);
    m_collections->m_discoverToolBar->setVisible(false);
    m_collections->disableVulnerabilityToolBar();
    m_collections->enableBookmarkToolBar();
    m_collections->enableScanSectionToolBar();
    m_collections->enableGlobalMenuToolBar();

    m_mainTabWidget->insertTab(0, m_scanWidget, QIcon(QString::fromUtf8(":/images/images/network_local.png")), "Scan");
    m_mainTabWidget->setCurrentIndex(0);

    if (!m_monitor->m_monitorWidget->isVisible()) {
        m_mainTabWidget->insertTab(1, m_monitor->m_monitorWidget, tr("Scan Monitor"));

        if (m_monitor->monitorHostNumber()) {
            m_mainTabWidget->setTabIcon(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget),
                                        QIcon::fromTheme("view-refresh", QIcon(":/images/images/reload.png")));
        } else {
            m_mainTabWidget->setTabIcon(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget),
                                        QIcon::fromTheme("utilities-system-monitor", QIcon(":/images/images/utilities-log-viewer.png")));
        }

    }

    m_mainTabWidget->insertTab(m_mainTabWidget->count(), m_bookmark->m_scanBookmarkWidget,
                     QIcon::fromTheme("user-bookmarks", QIcon(":/images/images/bookmark_folder.png")), tr("Bookmarks"));

    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_vulnerability->m_vulnerabilityWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_discoverManager->m_discoverWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_vulnBookmarkWidget));

    // enable scan action
    m_collections->enableBottomUiToggleActions();
    m_collections->enableScanBookmarkMenu();
}

void MainWindow::updateVulnerabilitySection()
{
    Notify::clearButtonNotify(m_collections->m_collectionsButton.value("vuln-sez"));
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(true);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(false);
    // main and action bar only in scan index
    m_collections->disableScanSectionToolBar();
    m_collections->enableVulnerabilityToolBar();
    m_collections->enableBookmarkToolBar();

    m_collections->enableGlobalMenuToolBar();
    m_collections->m_discoverToolBar->setVisible(false);

    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_scanBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_scanWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_discoverManager->m_discoverWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget));
    m_mainTabWidget->insertTab(0, m_vulnerability->m_vulnerabilityWidget, QIcon(QString::fromUtf8(":/images/images/viewmag+.png")), "Vulnerability");

    m_mainTabWidget->insertTab(m_mainTabWidget->count(), m_bookmark->m_vulnBookmarkWidget,
                               QIcon::fromTheme("user-bookmarks", QIcon(":/images/images/bookmark_folder.png")), tr("Bookmarks"));

    m_mainTabWidget->setCurrentIndex(0);

    // disable scan action
    m_collections->disableBottomUiToggleActions();
    m_collections->enableVulnerabilityBookmarkMenu();
}

void MainWindow::updateDiscoverSection()
{
    m_collections->m_collectionsButton.value("scan-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("vuln-sez")->setChecked(false);
    m_collections->m_collectionsButton.value("discover-sez")->setChecked(true);
    // main and action bar only in scan index
    m_collections->disableScanSectionToolBar();
    m_collections->disableBookmarkToolBar();
    m_collections->disableVulnerabilityToolBar();
    m_collections->m_discoverToolBar->setVisible(true);
    m_collections->enableGlobalMenuToolBar();

    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_scanBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_scanWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_vulnerability->m_vulnerabilityWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_bookmark->m_vulnBookmarkWidget));
    m_mainTabWidget->removeTab(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget));
    m_mainTabWidget->insertTab(0, m_discoverManager->m_discoverWidget, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")), "Network discover");
    m_mainTabWidget->setCurrentIndex(0);

    // disable scan action
    m_collections->disableBottomUiToggleActions();
    m_collections->disableBookmarkMenu();
}

void MainWindow::loadDefaultProfile()
{
    m_scanWidget->comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    m_scanWidget->comboAdv->insertItem(0, getParameters().join(" "));
}

void MainWindow::updateComboBook()
{
    m_scanWidget->comboHostBook->clear();
    m_scanWidget->comboHostBook->insertItem(0, "Select Saved Host");

    for (int index = 0; index < m_bookmark->m_scanBookmarkWidget->treeLogH->topLevelItemCount(); index++) {
        m_scanWidget->comboHostBook->insertItem(1, m_bookmark->m_scanBookmarkWidget->treeLogH->topLevelItem(index)->text(0));
    }
}

void MainWindow::clearAll()
{
    // Host list
    m_parser->clearParserItems();
    m_scanWidget->listScanError->clear();
    m_scanWidget->treeMain->clear();
    m_scanWidget->treeLookup->clear();
    m_scanWidget->treeTraceroot->clear();
    m_scanWidget->treeHostDet->clear();
    m_scanWidget->GItree->clear();
    m_scanWidget->listWscan->clear();
    m_scanWidget->treeNSS->clear();
    m_scanWidget->listScan->clear();
    m_collections->m_collectionsScanSection.value("clearHistory-action")->setEnabled(false);
    m_collections->disableSaveActions();

    m_scanWidget->comboScanLog->clear();
    m_scanWidget->comboScanLog->addItem(tr("Scan log parameters"));
}

void MainWindow::updateScanCounter(int hostNumber)
{
    if (hostNumber == 1) {
        m_mainTabWidget->setTabIcon(m_mainTabWidget->indexOf(m_monitor->m_monitorWidget),
                                    QIcon::fromTheme("view-refresh", QIcon(":/images/images/reload.png")));
    }

    QString title(tr(" Active Scan ")
                  + QLatin1String("(")
                  + QString("%1").arg(m_monitor->monitorHostNumber())
                  + QLatin1String(")")
                  + " - Nmapsi4");

    setWindowTitle(title);
}

void MainWindow::clearHostnameCombo()
{
    // reset combo host bookmarks to default value
    m_scanWidget->comboHostBook->setCurrentIndex(0);
    m_scanWidget->hostEdit->clearEditText();
}

void MainWindow::clearParametersCombo()
{
    // reset saved profile or simply the default one
    m_scanWidget->comboParametersProfiles->setCurrentIndex(m_savedProfileIndex);
    comboParametersSelectedEvent();
}
