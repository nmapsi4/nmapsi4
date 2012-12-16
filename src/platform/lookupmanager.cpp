/*
Copyright 2009-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "lookupmanager.h"


LookupManager::LookupManager(const QString hostname)
    : m_host(hostname)
{
}

LookupManager::~LookupManager()
{
    killLookup();
}

void LookupManager::run()
{
    qRegisterMetaType<QHostInfo>("QHostInfo");
    m_info = QHostInfo::fromName(m_host);

    if (m_info.error() != QHostInfo::NoError
            && m_info.error() != QHostInfo::UnknownError
            && m_info.error() != QHostInfo::HostNotFound) {

        emit threadEnd(m_info, -1, m_host);
        return;
    }
    emit threadEnd(m_info, 1, m_host);
}

void LookupManager::killLookup()
{
    m_info.abortHostLookup(m_info.lookupId());
}
