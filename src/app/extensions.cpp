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

QStringList nmapClass::loadExtensions()
{
    // FIXME
    QStringList parameters = comboAdv->lineEdit()->text().split(' ');
    parameters.append(check_extensions());

    return parameters;
}

QStringList nmapClass::check_extensions()
{
    QStringList parameters;

    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(10); // start progress bar
    }

    if(NSSsupport)
    {
        parameters << "-A";
    }
    else
    {
        if (!uid)
        {
            parameters << "--traceroute";
        }
    }

    if (nseComboScript->currentIndex())
    {
        //set --script "xx,xx"
        if (parameters.contains("-A"))
        {
            parameters.removeAll("-A");
            if (!uid)
            {
                parameters << "--traceroute";
            }
        }
        QString tmpListScript_("--script=");
        QString tmpListParam_("--script-args=");
        QString tmpList_;
        QString tmpListArgs_;

        if (_nseManager->getActiveNseScript().size())
        {
            foreach (const QString &token, _nseManager->getActiveNseScript())
            {
                tmpList_.append(token);
                tmpList_.append(",");
            }
        }

        // load nse manual script
        if (!comboNseInv->lineEdit()->text().isEmpty())
        {
            QStringList manualNse_ = comboNseInv->lineEdit()->text().split(',');
            foreach (const QString &token, manualNse_)
            {
                tmpList_.append(token);
                tmpList_.append(",");
            }
        }

        if (tmpList_.size())
        {
            tmpList_.remove(' ');
            tmpList_.resize(tmpList_.size()-1);
            tmpListScript_.append(tmpList_);
        }
        else
        {
            tmpListScript_.clear();
            tmpListScript_.append("-sC");
        }

        if (!comboNsePar->lineEdit()->text().isEmpty())
        {
            QString argsClean_ = comboNsePar->lineEdit()->text().remove('"');
            argsClean_ = argsClean_.remove('\'');
            QStringList argsNse_ = argsClean_.split(',');
            foreach (const QString &token, argsNse_)
            {
                tmpListArgs_.append(token);
                tmpListArgs_.append(",");
            }
        }

        if (tmpListArgs_.size())
        {
            tmpListArgs_.remove(' ');
            tmpListArgs_.resize(tmpListArgs_.size()-1);
            tmpListParam_.append(tmpListArgs_);
            parameters << tmpListParam_;
        }

        parameters << tmpListScript_;
    }

    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(55);
    }

    return parameters;
}
