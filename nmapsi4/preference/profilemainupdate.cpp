/***************************************************************************
 *   Copyright (C) 2007-2010 by Francesco Cecconi                          *
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

#include "profilemain.h"

void mainProfile::updateNormalCheck()   // slot
{

    if (checkNormalScan->isChecked()) {
        checkQuickScan->setChecked(false);
        checkFullVersion->setChecked(false);
        checkQuickVersion->setChecked(false);
        ScanActive = "normal";
    }
}

void mainProfile::updateQuickCheck()   //slot
{

    if (checkQuickScan->isChecked()) {
        checkNormalScan->setChecked(false);
        checkFullVersion->setChecked(false);
        checkQuickVersion->setChecked(false);
        ScanActive = "quick";
    }
}

void mainProfile::updateFullVersionCheck()   // slot
{

    if (checkFullVersion->isChecked()) {
        checkNormalScan->setChecked(false);
        checkQuickScan->setChecked(false);
        checkQuickVersion->setChecked(false);
        ScanActive = "fullversion";
    }
}

void mainProfile::updateQuickVersionCheck()   // slot
{

    if (checkQuickVersion->isChecked()) {
        checkNormalScan->setChecked(false);
        checkQuickScan->setChecked(false);
        checkFullVersion->setChecked(false);
        ScanActive = "quickversion";
    }
}

void mainProfile::update_saveButton()
{
    if (!checkLogOn->isChecked()) {
        lineEditPath->setEnabled(true);
        buttonLogB->setEnabled(true);
    } else {
        lineEditPath->setEnabled(false);
        buttonLogB->setEnabled(false);
    }
}

void mainProfile::activeLookupInt() {

    if(checkBoxlookup->isChecked())
        checkBoxDig->setChecked(false);
}

void mainProfile::activeLookupDig() {
    if(checkBoxDig->isChecked())
        checkBoxlookup->setChecked(false);
}

