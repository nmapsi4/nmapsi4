/***************************************************************************
 *   Copyright (C) 2008-2011 by Francesco Cecconi                          *
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


QString mwClass::showBrowser()
{

    QString url = QFileDialog::getOpenFileName(this, tr("Open Log"),
                  QDir::homePath(), tr("Log (*.log)"));

    return url;
}

void mwClass::itemDeleteAll(QList<QTreeWidgetItem*> items)
{
#ifndef LOGR_NO_DEBUG
    qDebug() << "nwClass/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void mwClass::exit()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("window-logr/pos", pos());
    settings.setValue("window-logr/size", size());
    settings.setValue("splitterSizesLogr", cW->saveState());
    close();
}


void mwClass::logFromHistory()
{
    if (logTree->currentItem()) {
        url = logTree->currentItem()->text(1);
        url.append(logTree->currentItem()->text(0));
        logReader();
    }
}

void mwClass::about()
{
    mainAbout about;
    about.exec();
}

void mwClass::about_qt()
{
    mainAbout about;
    about.qt();
}
