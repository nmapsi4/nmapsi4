/***************************************************************************
 *   Copyright (C) 2010.2011 by Francesco Cecconi                          *
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

// Getters
QString parserObj::getUptime() {
    if(uptime_.startsWith(" ")) {
        uptime_.remove(0,1);
    }
    return this->uptime_;

}

QString parserObj::getTcpSequence() {
    return this->tcp_sequence_;
}

QString parserObj::getDeviceType() {
    return this->deviceType_;
}

QString parserObj::getRunning() {
    return this->running_;
}

QStringList parserObj::getServices() {
    return this->services_;
}

QStringList parserObj::getDescriptionServices() {
    return this->descriptionServices_;
}

QStringList parserObj::getPortServices() {
    return this->portServices_;
}

QStringList parserObj::getPortOpen() {
    return this->portOpened_;
}

QStringList parserObj::getPortClose() {
    return this->portClosed_;
}

QStringList parserObj::getPortFiltered() {
    return this->portFiltered_;
}

QStringList parserObj::getScanInfo() {
    return this->scanInfo_;
}

QStringList parserObj::getNssInfo() {
    return this->scanNss_;
}

QStringList parserObj::getTraceRouteInfo() {
    return this->scanTraceRoute_;
}

QStringList parserObj::getFullScanLog() {
    return this->fullLogScan_;
}

QStringList parserObj::getErrorScan() {
    return this->errorScan_;
}

// Setters
void parserObj::setUptime(const QString uptime) {
    uptime_.append(uptime);

}

void parserObj::setTcpSequence(const QString tcpSequence) {
    tcp_sequence_.append(tcpSequence);
}

void parserObj::setDeviceType(const QString deviceType) {
    deviceType_.append(deviceType);
}

void parserObj::setRunning(const QString running) {
    running_.append(running);
}

void parserObj::setServices(const QString service) {
    services_.push_back(service);
}

void parserObj::setDescriptionServices(const QString desService) {
    descriptionServices_.push_back(desService);
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

void parserObj::setScanInfo(const QString infoElem) {
    scanInfo_.push_back(infoElem);
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
