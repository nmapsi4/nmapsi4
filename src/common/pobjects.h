/*
Copyright 2010-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef POBJECTS_H
#define POBJECTS_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QMetaType>

class PObject
{

public:
    PObject();
    ~PObject();

    const QString &getHostName() const;
    const QString &getParameters() const;
    const QString &scanDate() const;
    const QStringList &getHostInfo() const;
    const QStringList &getServices() const;
    const QStringList &getPortOpen() const;
    const QStringList &getPortClose() const;
    const QStringList &getPortFiltered() const;
    const QStringList &getTraceRouteInfo() const;
    const QStringList &getFullScanLog() const;
    const QStringList &getErrorScan() const;
    const QStringList &getVulnDiscoverd() const;
    const QHash<QString, QStringList> &getNseResult() const;
    bool isValidObject();
    int getId();

    void setHostName(const QString hostName);
    void setHostInfo(const QString hostInfoLine);
    void setScanDate(const QString date);
    void setServices(const QString Service);
    void setPortOpen(const QString portOpen);
    void setPortClose(const QString portClose);
    void setPortFiltered(const QString portFiltered);
    void setTraceRouteInfo(const QString traceElem);
    void setFullScanLog(const QString logElem);
    void setErrorScan(const QString errorElem);
    void setNseResult(const QHash<QString, QStringList> nseResult);
    void setValidity(bool isValid);
    void setParameters(const QString parameters);
    void setId(int id);
    void setVulnDiscoverd(const QString vulnAddress);

private:
    QString m_hostName;
    QString m_parameters;
    QString m_scanDate;
    QStringList m_mainInfo;
    QStringList m_services;
    QStringList m_portOpened;
    QStringList m_portFiltered;
    QStringList m_portClosed;
    QStringList m_scanTraceRoute;
    QStringList m_fullLogScan;
    QStringList m_errorScan;
    QHash<QString, QStringList> m_nssResult;
    QStringList m_vulnDiscoverd;
    bool m_validFlag;
    int m_id;

};

Q_DECLARE_METATYPE(PObject);

class PObjectLookup
{

public:
    PObjectLookup();
    ~PObjectLookup();

    const QStringList &getInfoLookup() const;
    const QString &getHostName() const;
    int getId();

    void setInfoLookup(const QString lookupElem);
    void setHostName(const QString hostName);
    void setId(int indexCounter);

private:
    QStringList m_mainLookup;
    QString m_hostName;
    int m_id;

};

Q_DECLARE_METATYPE(PObjectLookup);

#endif
