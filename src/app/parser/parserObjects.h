/***************************************************************************
 *   Copyright (C) 2010-2011 by Francesco Cecconi                          *
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

#ifndef PARSEROBJECTS_H
#define PARSEROBJECTS_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

/**
 * TODO:: make QStringList function const &.
 **/

class parserObj
{

public:
    parserObj();
    ~parserObj();

    const QString getHostName();
    const QStringList getMainInfo();
    const QStringList getServices();
    const QStringList getPortServices();
    const QStringList getPortOpen();
    const QStringList getPortClose();
    const QStringList getPortFiltered();
    const QStringList getTraceRouteInfo();
    const QStringList getFullScanLog();
    const QStringList getErrorScan();
    const QHash<QString, QStringList> getNseResult();

    void setHostName(const QString hostName);
    void setMainInfo(const QString infoMainElem);
    void setServices(const QString Service);
    void setPortServices(const QString portService);
    void setPortOpen(const QString portOpen);
    void setPortClose(const QString portClose);
    void setPortFiltered(const QString portFiltered);
    void setTraceRouteInfo(const QString traceElem);
    void setFullScanLog(const QString logElem);
    void setErrorScan(const QString errorElem);
    void setNseResult(const QHash<QString, QStringList> nseResult);

private:
    QString m_hostName;
    QStringList m_mainInfo;
    QStringList m_services;
    QStringList m_portServices;
    QStringList m_portOpened;
    QStringList m_portFiltered;
    QStringList m_portClosed;
    QStringList m_scanTraceRoute;
    QStringList m_fullLogScan;
    QStringList m_errorScan;
    QHash<QString, QStringList> m_nssResult;

};

class parserObjUtil
{

public:
    parserObjUtil();
    ~parserObjUtil();

    const QStringList getInfoLookup();
    const QString getHostName();

    void setInfoLookup(const QString lookupElem);
    void setHostName(const QString hostName);

private:
    QStringList m_mainLookup;
    QString m_hostName;

};

#endif
