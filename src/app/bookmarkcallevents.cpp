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

#include "mainwin.h"

void nmapClass::callSearchHistory()
{
    if (!actionAdd_Bookmark->isEnabled())
    {
        actionAdd_Bookmark->setEnabled(true);
    }

    if (_hostModel.isNull())
    {
        return;
    }

    if (_completer.isNull())
    {
        _completer = new QCompleter(_hostModel.data());
        _completer.data()->setCompletionRole(QCompleter::InlineCompletion);
        _completer.data()->setCaseSensitivity(Qt::CaseInsensitive);
        _completer.data()->setWrapAround(false);
        hostEdit->setCompleter(_completer.data());
    }
}

void nmapClass::callScanH()
{
     if(treeLogH->currentItem())
     {
        updateFontHost();
        // clear history setItemText fails
        hostEdit->insertItem(0, treeLogH->currentItem()->text(0));
        SWscan->setCurrentIndex(0);
        startScan();
    }
}

void nmapClass::saveBookMarks()
{
    if(hostEdit->currentText().isEmpty() && comboVulnRis->currentText().isEmpty())
    {
        return;
    }

    history *history_;

    if (tabScan->isVisible())
    {
        // save list of ip/dns in input
        history_ = new history(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);

        QStringList hostList = hostEdit->currentText().split(" ", QString::KeepEmptyParts);

        foreach (const QString& host, hostList)
        {
            history_->addItemHistory(host, QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));
        }

        freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeloghlist);
        m_treeloghlist = history_->updateBookMarks();
    }
    else
    {
        history_ = new history(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
        history_->addItemHistory(comboVulnRis->currentText(),
                                 QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));

        freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookvulnlist);
        m_treebookvulnlist = history_->updateBookMarks();
    }

    delete history_;
    updateComboBook();
}

void nmapClass::deleteBookMark()
{
    if(!treeLogH->currentItem() && !treeBookVuln->currentItem())
    {
        return;
    }

    history *history_;

    if (tabScan->isVisible())
    {
        history_ = new history(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
        history_->deleteItemBookmark(treeLogH->currentItem()->text(0));

        freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeloghlist);
        m_treeloghlist = history_->updateBookMarks();
    }
    else
    {
        history_ = new history(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
        history_->deleteItemBookmark(treeBookVuln->currentItem()->text(0));

        freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookvulnlist);
        m_treebookvulnlist = history_->updateBookMarks();
    }

    delete history_;
    updateComboBook();
}

void nmapClass::deleteBookMarkPar()
{

    if(!treeBookPar->currentItem())
    {
        return;
    }

    history *history_;

    if (!uid)
    {
        history_ = new history(treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
        history_->deleteItemBookmark(treeBookPar->currentItem()->text(0));
    }
    else
    {
        history_ = new history(treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
        history_->deleteItemBookmark(treeBookPar->currentItem()->text(0));
    }

    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookparlist);
    m_treebookparlist = history_->updateBookMarks();

    delete history_;
    loadScanProfile();
}

void nmapClass::startAddParBook_ui()
{
    const QString& parameters = comboAdv->currentText();

    if (parameters.isEmpty())
    {
        return;
    }

    QWeakPointer<addParametersToBookmark> dialogParAdd = new addParametersToBookmark(parameters);

    connect(dialogParAdd.data(), SIGNAL(doneParBook(QString,QString)),
            this, SLOT(saveBookMarksPar(QString,QString)));

    dialogParAdd.data()->exec();

    if (!dialogParAdd.isNull())
    {
        delete dialogParAdd.data();
    }
}

void nmapClass::saveBookMarksPar(const QString profileName, const QString profilePar)
{
    if(comboAdv->currentText().isEmpty())
    {
        return;
    }

    history *history_;

    if (!uid)
    {
        history_ = new history(treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);

        if (!history_->isProfileInHistory(profileName))
        {
            history_->addItemHistory(profilePar, profileName);
        }
        else
        {
            history_->updateProfile(profilePar,profileName);
        }
    }
    else
    {
        history_ = new history(treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);

        if (!history_->isProfileInHistory(profileName))
        {
            history_->addItemHistory(profilePar, profileName);
        }
        else
        {
            history_->updateProfile(profilePar,profileName);
        }
    }

    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treebookparlist);
    m_treebookparlist = history_->updateBookMarks();

    delete history_;
    loadScanProfile();
}

void nmapClass::slotHostSelected()
{
    if(comboHostBook->currentIndex())
    {
        updateFontHost();
        hostEdit->insertItem(0, comboHostBook->currentText());
    }
}
