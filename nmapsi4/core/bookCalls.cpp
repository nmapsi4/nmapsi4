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

#include "../mainwin.h"

void nmapClass::callSearchHistory()
{
    if (!actionAdd_Bookmark->isEnabled()) {
        actionAdd_Bookmark->setEnabled(true);
        action_Add_BookmarkToolBar->setEnabled(true);
    }
    logHistory *history = new logHistory("nmapsi4/cacheHost", hostCache);
    history->searchHistory(hostEdit->currentText(), hostEdit);
    delete history;
}

void nmapClass::callScanH()
{
     if(treeLogH->currentItem()) {
#ifndef TOOLS_NO_DEBUG
	qDebug() << "Current Item::" << treeLogH->currentItem();
#endif
        updateFontHost();
	// clear history setItemText fails
	hostEdit->insertItem(0, treeLogH->currentItem()->text(0));
        SWscan->setCurrentIndex(0);
        startScan();
    }
}

void nmapClass::saveBookMarks()
{
    if(hostEdit->currentText().isEmpty() && comboVulnRis->currentText().isEmpty()) {
        return;
    }

    logHistory *history_ = NULL;

    switch(stackedMain->currentIndex()) {
      case 0:
        history_ = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
        history_->addItemHistory(hostEdit->currentText(),
                                 QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));
        break;
      case 2:
        history_ = new logHistory(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
        history_->addItemHistory(comboVulnRis->currentText(),
                                 QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));
        break;
      default:
        break;
    }

    Bbook->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
    history_->updateBookMarks();
    delete history_;
    updateComboBook();
}

void nmapClass::deleteBookMark()
{
    if(!treeLogH->currentItem() && !treeBookVuln->currentItem()) {
        return;
    }

    logHistory *history_ = NULL;

    switch(stackedMain->currentIndex()) {
      case 0:
        history_ = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
        history_->deleteItemBookmark(treeLogH->currentItem()->text(0));
        break;
      case 2:
        history_ = new logHistory(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
        history_->deleteItemBookmark(treeBookVuln->currentItem()->text(0));
        break;
      default:
        break;
    }

    delete history_;
    updateComboBook();
}

void nmapClass::deleteBookMarkPar()
{

    if(!treeBookPar->currentItem()) {
        return;
    }

    logHistory *history_ = NULL;

    if (!uid) {
	history_ = new logHistory(treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
	history_->deleteItemBookmark(treeBookPar->currentItem()->text(0));
    } else {
	history_ = new logHistory(treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
	history_->deleteItemBookmark(treeBookPar->currentItem()->text(0));
    }

    delete history_;
    updateComboPar();
}

void nmapClass::saveBookMarksPar()
{

    if(comboAdv->currentText().isEmpty()) {
        return;
    }

    logHistory *history_ = NULL;

    if (!uid) {
	history_ = new logHistory(treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
	history_->addItemHistory(comboAdv->currentText(),
				QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));
    } else {
	history_ = new logHistory(treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
	history_->addItemHistory(comboAdv->currentText(),
				QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));
    }

    BBPar->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
    history_->updateBookMarks();
    delete history_;
    updateComboPar();
}

void nmapClass::callParFromBook()
{
    if(!frameAdv->isVisible()) {
        frameAdv->setVisible(true);
    }

     if(treeBookPar->currentItem()) {
	comboAdv->clear();
        comboAdv->insertItem(0, treeBookPar->currentItem()->text(0));
    }
}

void nmapClass::slotParSelected() {
    
   if(comboPar->currentIndex()) {
	comboAdv->clear();
	comboAdv->insertItem(0, comboPar->currentText());
   } else {
	comboAdv->insertItem(0, check_extensions().join(" "));
   }
}

void nmapClass::slotHostSelected() {
    
    if(comboHostBook->currentIndex()) {
	updateFontHost();
	hostEdit->insertItem(0, comboHostBook->currentText());
    }
}
