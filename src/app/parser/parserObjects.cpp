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

#include "parserObjects.h"

parserObj::parserObj()
{

}

parserObj::~parserObj()
{

}

const QString &parserObj::getHostName() const
{
    return m_hostName;
}

const QStringList &parserObj::getMainInfo() const
{
    return m_mainInfo;
}

const QStringList &parserObj::getServices() const
{
    return m_services;
}

const QStringList &parserObj::getPortServices() const
{
    return m_portServices;
}

const QStringList &parserObj::getPortOpen() const
{
    return m_portOpened;
}

const QStringList &parserObj::getPortClose() const
{
    return m_portClosed;
}

const QStringList &parserObj::getPortFiltered() const
{
    return m_portFiltered;
}

const QStringList &parserObj::getTraceRouteInfo() const
{
    return m_scanTraceRoute;
}

const QStringList &parserObj::getFullScanLog() const
{
    return m_fullLogScan;
}

const QStringList &parserObj::getErrorScan() const
{
    return m_errorScan;
}

const QHash< QString, QStringList > &parserObj::getNseResult() const
{
    return m_nssResult;
}

bool parserObj::isValidObject()
{
    return m_validFlag;
}

const QString &parserObj::getParameters() const
{
    return m_parameters;
}

void parserObj::setValidity(bool isValid)
{
    m_validFlag = isValid;
}

void parserObj::setHostName(const QString hostName)
{
    m_hostName.append(hostName);
}

void parserObj::setMainInfo(const QString elemMainInfo)
{
    m_mainInfo.append(elemMainInfo);
}

void parserObj::setServices(const QString service)
{
    m_services.push_back(service);
}

void parserObj::setPortServices(const QString portService)
{
    m_portServices.push_back(portService);
}

void parserObj::setPortOpen(const QString portOpen)
{
    m_portOpened.push_back(portOpen);
}

void parserObj::setPortClose(const QString portClose)
{
    m_portClosed.push_back(portClose);
}

void parserObj::setPortFiltered(const QString portFiltered)
{
    m_portFiltered.push_back(portFiltered);
}

void parserObj::setTraceRouteInfo(const QString traceElem)
{
    m_scanTraceRoute.push_back(traceElem);
}

void parserObj::setFullScanLog(const QString logElem)
{
    m_fullLogScan.push_back(logElem);
}

void parserObj::setErrorScan(const QString errorElem)
{
    m_errorScan.push_back(errorElem);
}

void parserObj::setParameters(const QString parameters)
{
    m_parameters = parameters;
}

parserObjUtil::parserObjUtil()
{

}

parserObjUtil::~parserObjUtil()
{

}

const QStringList &parserObjUtil::getInfoLookup() const
{
    return m_mainLookup;
}

const QString &parserObjUtil::getHostName() const
{
    return m_hostName;
}

void parserObjUtil::setInfoLookup(const QString lookupElem)
{
    m_mainLookup.push_back(lookupElem);
}

void parserObjUtil::setHostName(const QString hostName)
{
    m_hostName.append(hostName);
}

void parserObj::setNseResult(const QHash< QString, QStringList > nseResult)
{
    m_nssResult = nseResult;
}
