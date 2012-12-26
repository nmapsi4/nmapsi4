/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "profilehandler.h"
#include "mainwindow.h"

ProfileHandler::ProfileHandler(MainWindow* ui, int userId, int defaultProfile) :
    QObject(ui),
    m_userId(userId),
    m_savedProfileIndex(defaultProfile),
    m_ui(ui)
{

}

QList< QPair<QString, QString> > ProfileHandler::defaultScanProfile() const
{
    // preload StringList with default static Scan profile
    QPair<QString, QString> profileModel;
    QList< QPair<QString, QString> > listProfileModel;

    if (!m_userId) {
        profileModel.first = tr("Default");
        profileModel.second = "-sS -sV -O -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Default, force ipv6");
        profileModel.second = "-sS -sV -O -T4 -v -6 --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Default, Aggressive");
        profileModel.second = "-A -sS -sV -O -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Default, base nse script");
        profileModel.second = "--script=default,safe -sS -sV -O -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Default, base nse script, force ipv6");
        profileModel.second = "--script=default,safe -sS -sV -O -T4 -v -6 --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Quick Scan");
        profileModel.second = "-T4 --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Intense Scan");
        profileModel.second = "-T4 -A -v -PE -PS22,25,80 -PA21,23,80,3389 --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Intense Scan, no ping");
        profileModel.second = "-T4 -A -v -P0 --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Intense Scan, all TCP ports");
        profileModel.second = "-T4 -A -v -PE -PS22,25,80 -PA21,23,80,3389 --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Intense scan plus UDP");
        profileModel.second = "-sS -sU -T4 -v --traceroute";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Slow comprehensive scan");
        profileModel.second = "-sS -sU -T4 -v -PE -PP -PS80,443 -PA3389 -PU40125 -PY -g 53 --traceroute";
        listProfileModel.push_back(profileModel);
    } else {
        // for user mode
        profileModel.first = tr("Default");
        profileModel.second = "-sT -sV -T4 -v";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Default, force ipv6");
        profileModel.second = "-sT -sV -T4 -v -6";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Default, Aggressive");
        profileModel.second = "-A -sT -sV -T4 -v";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Default, base nse script");
        profileModel.second = "--script=default,safe -sT -sV -T4 -v";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Default, base nse script, force ipv6");
        profileModel.second = "--script=default,safe -sT -sV -T4 -v -6";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Quick Scan");
        profileModel.second = "-T4";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Intense Scan");
        profileModel.second = "-T4 -A -v -PS22,25,80 -PA21,23,80,3389";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Intense Scan, no ping");
        profileModel.second = "-T4 -A -v -P0";
        listProfileModel.push_back(profileModel);

        profileModel.first = tr("Intense Scan, all TCP ports");
        profileModel.second = "-T4 -A -PS22,25,80 -PA21,23,80,3389 -v";
        listProfileModel.push_back(profileModel);
    }

    return listProfileModel;
}

QStringList ProfileHandler::getParameters() const
{
    return m_ui->m_scanWidget->comboAdv->lineEdit()->text().split(' ');
}

bool ProfileHandler::containsParameter(const QString& parameter) const
{
    return getParameters().contains(parameter) ? true : false;
}

void ProfileHandler::updateComboParametersFromList(const QStringList& parameters)
{
    m_ui->m_scanWidget->comboAdv->clear();
    m_ui->m_scanWidget->comboAdv->insertItem(0, parameters.join(" "));
}

void ProfileHandler::loadDefaultProfile()
{
    m_ui->m_scanWidget->comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    m_ui->m_scanWidget->comboAdv->insertItem(0, getParameters().join(" "));
}

void ProfileHandler::clearParametersCombo()
{
    // reset saved profile or simply the default one
    m_ui->m_scanWidget->comboParametersProfiles->setCurrentIndex(m_savedProfileIndex);
    m_ui->comboParametersSelectedEvent();
}
