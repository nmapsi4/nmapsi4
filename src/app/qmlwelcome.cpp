/*
Copyright 2013  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "qmlwelcome.h"
#include "mainwindow.h"

QmlWelcome::QmlWelcome(MainWindow* parent)
    :QObject(parent), m_ui(parent)
{

}

void QmlWelcome::scanSection()
{
    m_ui->updateScanSection();
}

void QmlWelcome::discoverSection()
{
    m_ui->updateDiscoverSection();
}

void QmlWelcome::vulnerabilitySection()
{
    m_ui->updateVulnerabilitySection();
}

void QmlWelcome::callScan(const QString hostName)
{
    QHostAddress addressProtocol(hostName);
    if ((addressProtocol.protocol() != QAbstractSocket::IPv4Protocol)
            && (addressProtocol.protocol() != QAbstractSocket::IPv6Protocol)
            && (!HostTools::isValidDns(hostName))) {
        return;
    }

    m_ui->updateComboHostnameProperties();
    m_ui->m_scanWidget->hostEdit->insertItem(0, hostName);
    m_ui->updateScanSection();
}
