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

parserObj::parserObj() {

}

parserObj::~parserObj() {

}

QString parserObj::getHostName() const {
    return this->hostName_;
}

QStringList parserObj::getMainInfo() const {
    return this->mainInfo_;
}

QStringList parserObj::getServices() const {
    return this->services_;
}

QStringList parserObj::getPortServices() const {
    return this->portServices_;
}

QStringList parserObj::getPortOpen() const {
    return this->portOpened_;
}

QStringList parserObj::getPortClose() const {
    return this->portClosed_;
}

QStringList parserObj::getPortFiltered() const {
    return this->portFiltered_;
}

QStringList parserObj::getNssInfo() const {
    return this->scanNss_;
}

QStringList parserObj::getTraceRouteInfo() const {
    return this->scanTraceRoute_;
}

QStringList parserObj::getFullScanLog() const {
    return this->fullLogScan_;
}

QStringList parserObj::getErrorScan() const {
    return this->errorScan_;
}

void parserObj::setHostName(const QString hostName) {
    hostName_.append(hostName);
}

void parserObj::setMainInfo(const QString elemMainInfo) {
    mainInfo_.append(elemMainInfo);
}

void parserObj::setServices(const QString service) {
    services_.push_back(service);
}

void parserObj::setPortServices(const QString portService) {
    portServices_.push_back(portService);
}

void parserObj::setPortOpen(const QString portOpen) {
    portOpened_.push_back(portOpen);
}

void parserObj::setPortClose(const QString portClose) {
    portClosed_.push_back(portClose);
}

void parserObj::setPortFiltered(const QString portFiltered) {
    portFiltered_.push_back(portFiltered);
}

void parserObj::setNssInfo(const QString nssElem) {
    scanNss_.push_back(nssElem);
}

void parserObj::setTraceRouteInfo(const QString traceElem) {
    scanTraceRoute_.push_back(traceElem);
}

void parserObj::setFullScanLog(const QString logElem) {
    fullLogScan_.push_back(logElem);
}

void parserObj::setErrorScan(const QString errorElem) {
    errorScan_.push_back(errorElem);
}
