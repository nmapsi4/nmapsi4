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

QList< QPair<QString, QString> > MainWindow::defaultScanProfile()
{
    // preload StringList with default static Scan profile
    QPair<QString, QString> profileModel;
    QList< QPair<QString, QString> > listProfileModel;

    if (!m_userId)
    {
        profileModel.first = "Default";
        profileModel.second = "-sS -sV -O -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = "Default + Aggressive";
        profileModel.second = "-A -sS -sV -O -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = "Default + base nse script";
        profileModel.second = "--script=default,safe -sS -sV -O -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        //FIXME tmpStaticProfileMap_.insert(tr("Quick Scan"),"-T4 -F --traceroute");
        profileModel.first = tr("Quick Scan");
        profileModel.second = "-T4 -F --traceroute";
        listProfileModel.push_back(profileModel);

        //FIXME tmpStaticProfileMap_.insert(tr("Intense Scan"),"-T4 -v --traceroute");
        profileModel.first = tr("Intense Scan");
        profileModel.second = "-T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        //FIXME tmpStaticProfileMap_.insert(tr("Intense Scan, all TCP ports"),"-p 1-65535 -T4 -v --traceroute");
        profileModel.first = tr("Intense Scan, all TCP ports");
        profileModel.second = "-p 1-65535 -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Intense scan plus UDP");
        profileModel.second = "-sS -sU -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Slow comprehensive scan");
        profileModel.second = "-sS -sU -T4 -v -PE -PP -PS80,443 -PA3389 -PU40125 -PY -g 53 --traceroute";
        listProfileModel.push_back(profileModel);
    }
    else
    {
        // for user mode

        profileModel.first = "Default";
        profileModel.second = "-sT -sV -T4 -v";
        listProfileModel.push_back(profileModel);

        profileModel.first = "Default + Aggressive";
        profileModel.second = "-A -sT -sV -T4 -v";
        listProfileModel.push_back(profileModel);

        profileModel.first = "Default + base nse script";
        profileModel.second = "--script=default,safe -sT -sV -T4 -v";
        listProfileModel.push_back(profileModel);

        //tmpStaticProfileMap_.insert(tr("Intense Scan, all TCP ports"),"-p 1-65535 -T4 -v");
        profileModel.first = tr("Intense Scan, all TCP ports");
        profileModel.second = "-p 1-65535 -T4 -v";
        listProfileModel.push_back(profileModel);

        //FIXME tmpStaticProfileMap_.insert(tr("Intense Scan"),"-T4 -v");
        profileModel.first = tr("Intense Scan");
        profileModel.second = "-T4 -v";
        listProfileModel.push_back(profileModel);

        //FIXME tmpStaticProfileMap_.insert(tr("Quick Scan"),"-T4 -F");
        profileModel.first = tr("Quick Scan");
        profileModel.second = "-T4 -F";
        listProfileModel.push_back(profileModel);
    }

    return listProfileModel;
}

void MainWindow::loadScanProfile()
{
    comboParametersProfiles->clear();

    QListIterator< QPair<QString, QString> > i(defaultScanProfile());
    while (i.hasNext())
    {
        comboParametersProfiles->insertItem(comboParametersProfiles->count()+1, i.next().first);
    }

    comboParametersProfiles->insertSeparator(comboParametersProfiles->count()+1);

    // value from treeWidget parameters
    for(int index=0; index < treeBookPar->topLevelItemCount(); index++)
    {
        comboParametersProfiles->addItem(treeBookPar->topLevelItem(index)->text(1));
    }

    comboParametersSelectedEvent();
}

void MainWindow::comboParametersSelectedEvent()
{
   // insert profile from comboPar to comboAdv
   int parIndex = comboParametersProfiles->currentIndex();

    // if not 0
    QList< QPair<QString, QString> > listProfileModel = defaultScanProfile();
    comboAdv->clear();

    if (parIndex <= listProfileModel.size())
    {
        QListIterator< QPair<QString, QString> > i(defaultScanProfile());
        while (i.hasNext())
        {
            QPair<QString, QString> profile = i.next();
            if (profile.first == comboParametersProfiles->currentText())
            {
                // call static default profile for check
                comboAdv->insertItem(0, profile.second);
                break;
            }
        }
    }
    else
    {
        // saved user profile
        QList<QTreeWidgetItem *> resultList_ = treeBookPar->findItems(comboParametersProfiles->currentText(),Qt::MatchExactly,1);
        comboAdv->insertItem(0, resultList_[0]->text(0));
    }
}

void MainWindow::resetComboParameters()
{
    comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboParametersSelectedEvent();
}

QStringList MainWindow::loadExtensions()
{
    if(m_monitor->monitorHostNumber())
    {
        progressScan->setValue(55); // start progress bar
    }

    return comboAdv->lineEdit()->text().split(' ');
}
