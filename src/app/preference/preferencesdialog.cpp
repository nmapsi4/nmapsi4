/***************************************************************************
 *   Copyright (C) 2007-2012 by Francesco Cecconi                          *
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

#include "preferencesdialog.h"

preferencesDialog::preferencesDialog(QWidget *parent)
: QDialog(parent)
{
    setupUi(this);
    QSettings settings("nmapsi4", "nmapsi4");

    connect(buttonSave, SIGNAL(clicked(bool)),
            this, SLOT(quit()));
    connect(checkBoxlookup, SIGNAL(toggled(bool)),
            this, SLOT(activeLookupInt()));
    connect(checkBoxDig, SIGNAL(toggled(bool)),
            this, SLOT(activeLookupDig()));

    // create a read log config
    int logType = settings.value("logType", 0).toInt();
    comboLogType->setCurrentIndex(logType);

    // Restore max parallel scan option
    int maxParallelScan = settings.value("maxParallelScan", 5).toInt();
    spinMaxParallelScan->setValue(maxParallelScan);

    // Restore max discover process
    int maxDiscoverProcess = settings.value("maxDiscoverProcess", 20).toInt();
    spinMaxDiscoverProcess->setValue(maxDiscoverProcess);

    QString tmpSavePos = settings.value("savePos", "none").toString();
    if (tmpSavePos.contains("true"))
    {
        checkWinPos->setChecked(true);
    }
    else
    {
        checkWinPos->setChecked(false);
    }

    QString tmpSize = settings.value("saveSize", "none").toString();
    if (tmpSize.contains("true"))
    {
        checkSize->setChecked(true);
    }
    else
    {
        checkSize->setChecked(false);
    }

    bool lookI = settings.value("lookInternal").toBool();
    if (lookI)
    {
        checkBoxlookup->setChecked(true);
    }
    else
    {
        checkBoxlookup->setChecked(false);
    }

    bool lookD = settings.value("lookDig").toBool();
    if (lookD)
    {
        checkBoxDig->setChecked(true);
    }
    else
    {
        checkBoxDig->setChecked(false);
    }


    QString cache = settings.value("hostCache", "10").toString();
    spinBoxCache->setValue(cache.toInt());

    // Create listview items
    // TODO insert history item and window setup
    listViewOptions->setIconSize(QSize(52, 52));

    m_generalItem = new QListWidgetItem(listViewOptions);
    m_generalItem->setIcon(QIcon(QString::fromUtf8(":/images/images/tool.png")));
    m_generalItem->setText(tr("General"));

    m_sizeItem = new QListWidgetItem(listViewOptions);
    m_sizeItem->setIcon(QIcon(QString::fromUtf8(":/images/images/view-fullscreen.png")));
    m_sizeItem->setText(tr("Size"));

#ifndef Q_WS_WIN
    m_lookItem = new QListWidgetItem(listViewOptions);
    m_lookItem->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    m_lookItem->setText(tr("Lookup"));
#endif

    connect(listViewOptions, SIGNAL(itemSelectionChanged()),
            this, SLOT(updateItem()));

    m_generalItem->setSelected(true);

    connect(buttonDefault, SIGNAL(clicked()),
            this, SLOT(setDefaults()));

    // FIXME disable dig support (many works)
    //checkBoxDig->setEnabled(false);
}

preferencesDialog::~preferencesDialog()
{
    // NOTE: remove segfault on delete
    listViewOptions->disconnect(SIGNAL(itemSelectionChanged()));

    delete m_generalItem;
    delete m_lookItem;
    delete m_sizeItem;
}

void preferencesDialog::saveValues()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("logType", comboLogType->currentIndex());
    settings.setValue("hostCache", spinBoxCache->value());
    settings.setValue("maxParallelScan", spinMaxParallelScan->value());
    settings.setValue("maxDiscoverProcess", spinMaxDiscoverProcess->value());

    if (checkWinPos->isChecked())
    {
        settings.setValue("savePos", "true");
    }
    else
    {
        settings.setValue("savePos", "false");
    }

    if (checkSize->isChecked())
    {
        settings.setValue("saveSize", "true");
    }
    else
    {
        settings.setValue("saveSize", "false");
    }

#ifndef Q_WS_WIN
    if (checkBoxlookup->isChecked())
    {
        settings.setValue("lookInternal", "true");
    }
    else
    {
        settings.setValue("lookInternal", "false");
    }

    if (checkBoxDig->isChecked())
    {
        settings.setValue("lookDig", "true");
    }
    else
    {
        settings.setValue("lookDig", "false");
    }
#endif
}


void preferencesDialog::updateItem()
{
    if (m_generalItem->isSelected())
    {
        labelTitle->setText(tr("<h3>General</h3>"));
        stackPref->setCurrentIndex(0);
    }
    else if (m_sizeItem->isSelected())
    {
        labelTitle->setText(tr("<h3>Size</h3>"));
        stackPref->setCurrentIndex(1);
    }
    else if (m_lookItem->isSelected())
    {
        labelTitle->setText(tr("<h3>Lookup</h3>"));
        stackPref->setCurrentIndex(2);
    }
}

void preferencesDialog::quit()
{
    saveValues(); // save Options
    emit accept();   // send accept signal and exit
}

void preferencesDialog::setDefaults()
{
    checkSize->setChecked(false);
    checkWinPos->setChecked(false);
    checkBoxlookup->setChecked(true);
    checkBoxDig->setChecked(false);
}

void preferencesDialog::activeLookupInt()
{
    if(checkBoxlookup->isChecked())
    {
        checkBoxDig->setChecked(false);
    }
}

void preferencesDialog::activeLookupDig()
{
    if(checkBoxDig->isChecked())
    {
        checkBoxlookup->setChecked(false);
    }
}
