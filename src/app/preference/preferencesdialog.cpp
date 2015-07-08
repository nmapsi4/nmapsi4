/*
Copyright 2007-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    QSettings settings("nmapsi4", "nmapsi4");

    connect(buttonSave, &QPushButton::clicked,
            this, &PreferencesDialog::quit);
    connect(buttonDefault, &QPushButton::clicked,
            this, &PreferencesDialog::setDefaults);
    connect(comboLookupType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PreferencesDialog::updateLookupState);

    // create a read log config
    comboLogType->setCurrentIndex(settings.value("logType", 1).toInt());
    // Restore max parallel scan option
    spinMaxParallelScan->setValue(settings.value("maxParallelScan", 5).toInt());
    // Restore max discover process
    spinMaxDiscoverProcess->setValue(settings.value("maxDiscoverProcess", 20).toInt());
    comboLookupType->setCurrentIndex(settings.value("lookupType", 1).toInt());
    digVerbosityCombo->setCurrentIndex(settings.value("digVerbosityLevel", 0).toInt());
    spinBoxCache->setValue(settings.value("hostCache", 10).toInt());

    // Create listview items
    listViewOptions->setIconSize(QSize(42, 42));

    m_generalItem = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/tool.png")), tr("General"));
    listViewOptions->addItem(m_generalItem);

#if !defined(Q_OS_WIN32)
    m_lookItem = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/network_local.png")), tr("Lookup"));
    listViewOptions->addItem(m_lookItem);
#else
    //NOTE disable lookup on MS Windows
    comboLookupType->setCurrentIndex(0);
    comboLookupType->setDisabled(true);
#endif

    connect(listViewOptions, &QListWidget::itemSelectionChanged,
            this, &PreferencesDialog::updateListWidgetItem);

    m_generalItem->setSelected(true);
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::updateLookupState()
{
    if (comboLookupType->currentIndex() == 2) {
        digVerbosityCombo->setEnabled(true);
    } else {
        digVerbosityCombo->setEnabled(false);
    }
}

void PreferencesDialog::saveValues()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("logType", comboLogType->currentIndex());
    settings.setValue("hostCache", spinBoxCache->value());
    settings.setValue("maxParallelScan", spinMaxParallelScan->value());
    settings.setValue("maxDiscoverProcess", spinMaxDiscoverProcess->value());
    settings.setValue("lookupType", comboLookupType->currentIndex());
    settings.setValue("digVerbosityLevel", digVerbosityCombo->currentIndex());
}


void PreferencesDialog::updateListWidgetItem()
{
    if (m_generalItem->isSelected()) {
        labelTitle->setText(tr("<h3>General</h3>"));
        stackPref->setCurrentIndex(0);
    } else if (m_lookItem->isSelected()) {
        labelTitle->setText(tr("<h3>Lookup</h3>"));
        stackPref->setCurrentIndex(1);
    }
}

void PreferencesDialog::quit()
{
    saveValues(); // save Options
    emit accept();   // send accept signal and exit
}

void PreferencesDialog::setDefaults()
{
#if !defined(Q_OS_WIN32)
    comboLookupType->setCurrentIndex(1);
#else
    comboLookupType->setCurrentIndex(0);
#endif
}
