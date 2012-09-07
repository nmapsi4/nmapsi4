/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
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

#include "selectprofiledialog.h"

SelectProfileDialog::SelectProfileDialog(MainWindow* ui)
: QDialog(ui), m_ui(ui)
{
    setupUi(this);
    loadComboValues();

    connect(doneButton, SIGNAL(clicked(bool)),
            this, SLOT(closeDialog()));
}

SelectProfileDialog::~SelectProfileDialog()
{

}

void SelectProfileDialog::loadComboValues()
{
    showProfilesCombo->setModel(m_ui->comboParametersProfiles->model());
    showProfilesCombo->setCurrentIndex(m_ui->comboParametersProfiles->currentIndex());
}

void SelectProfileDialog::closeDialog()
{
    m_ui->comboParametersProfiles->setCurrentIndex(showProfilesCombo->currentIndex());
    m_ui->comboParametersSelectedEvent();
    close();
}
