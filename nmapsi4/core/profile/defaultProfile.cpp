/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
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

#include "../../mainwin.h"

QMap<QString, QString> nmapClass::defaultScanProfile() const {
    // preload StringList with default static Scan profile
    // use QMap::insert(key,value) for new value
    // use QMap::value(key) for value from a key
    QMap<QString, QString> tmpStaticProfileMap_;
    
    // for user mode
    tmpStaticProfileMap_.insert(tr("Quick Scan"),"-T4 -F --traceroot");
    tmpStaticProfileMap_.insert(tr("Intense Scan"),"-T4 -A -v --traceroot");
    tmpStaticProfileMap_.insert(tr("Intense Scan, all TCP ports"),"-p 1-65535 -T4 -A -v --traceroot");
    
    if (!uid) {
	// profile for full mode
	tmpStaticProfileMap_.insert(tr("Intense scan plus UDP"),"-sS -sU -T4 -A -v");
	tmpStaticProfileMap_.insert(tr("Slow comprehensive scan"),
				   "-sS -sU -T4 -A -v -PE -PP -PS80,443 -PA3389 -PU40125 -PY -g 53 --traceroot");
    }

    return tmpStaticProfileMap_;
}

void nmapClass::setQuickProfile()
{
    this->resetOptions();
    comboTiming->setCurrentIndex(4);
    comboDNSResolv->setCurrentIndex(0);
    comboVerbosity->setCurrentIndex(1);
}

void nmapClass::setNormalProfile()
{
    this->resetOptions();
    comboTiming->setCurrentIndex(4);
    comboDNSResolv->setCurrentIndex(0);
    comboVerbosity->setCurrentIndex(1);
    versionBox->setChecked(true);
}

void nmapClass::setFullVersionProfile()
{
    this->resetOptions();
    comboTiming->setCurrentIndex(4);
    checkOS->setChecked(true);
    versionBox->setChecked(true);
    comboVerbosity->setCurrentIndex(1);
    comboDNSResolv->setCurrentIndex(0);
}

void nmapClass::setQuickVersionProfile()
{
    this->resetOptions();
    comboTiming->setCurrentIndex(4);
    checkOS->setChecked(true);
    comboDNSResolv->setCurrentIndex(0);
}

void nmapClass::resetOptions()
{
    versionBox->setChecked(false);
    checkOS->setChecked(false);
    comboTiming->setCurrentIndex(0);
    comboDNSResolv->setCurrentIndex(0);
    comboVerbosity->setCurrentIndex(0);
}
