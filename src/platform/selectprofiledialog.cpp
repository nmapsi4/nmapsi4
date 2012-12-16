/*
Copyright 2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "selectprofiledialog.h"
#include "mainwindow.h"

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
    showProfilesCombo->setModel(m_ui->m_scanWidget->comboParametersProfiles->model());
    showProfilesCombo->setCurrentIndex(m_ui->m_scanWidget->comboParametersProfiles->currentIndex());
}

void SelectProfileDialog::closeDialog()
{
    m_ui->m_scanWidget->comboParametersProfiles->setCurrentIndex(showProfilesCombo->currentIndex());
    m_ui->comboParametersSelectedEvent();
    close();
}
