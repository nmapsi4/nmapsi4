/***************************************************************************
 *   Copyright (C) 2011-2012 by Francesco Cecconi                          *
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

#include "mainwindow.h"

QHash<QString, QString> MainWindow::defaultScanProfile() const
{
    // preload StringList with default static Scan profile
    QHash<QString, QString> tmpStaticProfileMap_;

    if (!uid)
    {
        tmpStaticProfileMap_.insert("Default","-sS -sV -O -T4 -v");
        tmpStaticProfileMap_.insert(tr("Quick Scan"),"-T4 -F");
        tmpStaticProfileMap_.insert(tr("Intense Scan"),"-T4 -v");
        tmpStaticProfileMap_.insert(tr("Intense Scan, all TCP ports"),"-p 1-65535 -T4 -v");
        // profile for full mode
        tmpStaticProfileMap_.insert(tr("Intense scan plus UDP"),"-sS -sU -T4 -v");
        tmpStaticProfileMap_.insert(tr("Slow comprehensive scan"),
                                    "-sS -sU -T4 -v -PE -PP -PS80,443 -PA3389 -PU40125 -PY -g 53");
    }
    else
    {
        // for user mode
        tmpStaticProfileMap_.insert("Default","-sT -sV -T4 -v");
        tmpStaticProfileMap_.insert(tr("Intense Scan, all TCP ports"),"-p 1-65535 -T4 -v");
        tmpStaticProfileMap_.insert(tr("Intense Scan"),"-T4 -v");
        tmpStaticProfileMap_.insert(tr("Quick Scan"),"-T4 -F");
    }

    return tmpStaticProfileMap_;
}

void MainWindow::loadScanProfile()
{
    // TODO create a default profile
    comboPar->clear();
    comboPar->insertItem(0, "Default");

    // insert default static profile
    QHash<QString, QString> tmpStaticProfile_ = defaultScanProfile();
    int keyPosition = 0;
    QHash<QString, QString>::const_iterator i;
    for (i = tmpStaticProfile_.constBegin(); i != tmpStaticProfile_.constEnd(); ++i)
    {
        //comboPar->insertItem(comboPosition, i.key());
        if (i.key() != "Default")
        {
            comboPar->addItem(i.key());
            keyPosition++;
        }
    }

    comboPar->insertSeparator(++keyPosition);

    // value from treeWidget parameters
    for(int index=0; index < treeBookPar->topLevelItemCount(); index++)
    {
        comboPar->addItem(treeBookPar->topLevelItem(index)->text(1));
    }

    slotParSelected();
}

void MainWindow::slotParSelected()
{
   // insert profile from comboPar to comboAdv
   int parIndex = comboPar->currentIndex();

    // if not 0
    QHash<QString, QString> tmpMap_ = defaultScanProfile();
    comboAdv->clear();
    if (parIndex <= tmpMap_.size())
    {
        // call static default profile for check
        comboAdv->insertItem(0, tmpMap_.value(comboPar->currentText()));
    }
    else
    {
        // saved user profile
        QList<QTreeWidgetItem *> resultList_ = treeBookPar->findItems(comboPar->currentText(),Qt::MatchExactly,1);
        comboAdv->insertItem(0, resultList_[0]->text(0));
    }
}

void MainWindow::resetPar()
{
    comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    slotParSelected();
}

QStringList MainWindow::loadExtensions()
{
    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(55); // start progress bar
    }

    return comboAdv->lineEdit()->text().split(' ');
}
