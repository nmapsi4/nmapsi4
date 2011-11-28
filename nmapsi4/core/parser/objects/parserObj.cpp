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

#include "parserObj.h"

parserObj::parserObj() 
{

}

parserObj::~parserObj() 
{

}

QString parserObj::getHostName() const 
{
    return this->m_hostName;
}

QStringList parserObj::getMainInfo() const 
{
    return this->m_mainInfo;
}

QStringList parserObj::getServices() const 
{
    return this->m_services;
}

QStringList parserObj::getPortServices() const 
{
    return this->m_portServices;
}

QStringList parserObj::getPortOpen() const 
{
    return this->m_portOpened;
}

QStringList parserObj::getPortClose() const 
{
    return this->m_portClosed;
}

QStringList parserObj::getPortFiltered() const 
{
    return this->m_portFiltered;
}

QStringList parserObj::getNssInfo() const 
{
    return this->m_scanNss;
}

QStringList parserObj::getTraceRouteInfo() const 
{
    return this->m_scanTraceRoute;
}

QStringList parserObj::getFullScanLog() const 
{
    return this->m_fullLogScan;
}

QStringList parserObj::getErrorScan() const 
{
    return this->m_errorScan;
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

void parserObj::setNssInfo(const QString nssElem) 
{
    m_scanNss.push_back(nssElem);
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
