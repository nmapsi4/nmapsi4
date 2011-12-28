/***************************************************************************
 *   Copyright (C) 2007-2011 by Francesco Cecconi                          *
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

preferencesDialog::preferencesDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    QSettings ptrFile("nmapsi4", "nmapsi4");

    connect(buttonSave, SIGNAL(clicked(bool)),
            this, SLOT(quit()));
    connect(checkNormalScan, SIGNAL(toggled(bool)),
            this, SLOT(updateNormalCheck()));
    connect(checkQuickScan, SIGNAL(toggled(bool)),
            this, SLOT(updateQuickCheck()));
    connect(checkFullVersion, SIGNAL(toggled(bool)),
            this, SLOT(updateFullVersionCheck()));
    connect(checkQuickVersion, SIGNAL(toggled(bool)),
            this, SLOT(updateQuickVersionCheck()));

    connect(checkBoxlookup, SIGNAL(toggled(bool)),
            this, SLOT(activeLookupInt()));
    connect(checkBoxDig, SIGNAL(toggled(bool)),
            this, SLOT(activeLookupDig()));

    setProfile(); // set profile preference

    // create a read log config
    int logType = ptrFile.value("logType", 0).toInt();
    comboLogType->setCurrentIndex(logType);

    QString tmpSavePos = ptrFile.value("savePos", "none").toString();
    if (tmpSavePos.contains("true"))
    {
        checkWinPos->setChecked(true);
    }
    else
    {
        checkWinPos->setChecked(false);
    }

    QString tmpSize = ptrFile.value("saveSize", "none").toString();
    if (tmpSize.contains("true"))
    {
        checkSize->setChecked(true);
    }
    else
    {
        checkSize->setChecked(false);
    }

    bool lookI = ptrFile.value("lookInternal").toBool();
    if (lookI)
    {
        checkBoxlookup->setChecked(true);
    }
    else
    {
        checkBoxlookup->setChecked(false);
    }

    bool lookD = ptrFile.value("lookDig").toBool();
    if (lookD)
    {
        checkBoxDig->setChecked(true);
    }
    else
    {
        checkBoxDig->setChecked(false);
    }


    QString cache = ptrFile.value("hostCache", "10").toString();
    spinBoxCache->setValue(cache.toInt());

    // Create listview items
    // TODO insert history item and window setup
    listViewOptions->setIconSize(QSize(52, 52));

    m_profileItem = new QListWidgetItem(listViewOptions);
    m_profileItem->setIcon(QIcon(QString::fromUtf8(":/images/images/preferences-system-windows.png")));
    m_profileItem->setText(tr("Profiles"));

    m_logItem = new QListWidgetItem(listViewOptions);
    m_logItem->setIcon(QIcon(QString::fromUtf8(":/images/images/utilities-log-viewer.png")));
    m_logItem->setText(tr("Log"));

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

    m_profileItem->setSelected(true);

    connect(buttonDefault, SIGNAL(clicked()),
            this, SLOT(setDefaults()));

    // FIXME disable dig support (many works)
    //checkBoxDig->setEnabled(false);

}

void preferencesDialog::saveProfile(const QString ProfileType)
{
    QSettings ptrFile("nmapsi4", "nmapsi4");
    ptrFile.setValue("configProfile", ProfileType); // default value
}

QString preferencesDialog::readProfile()
{
    QSettings ptrFile("nmapsi4", "nmapsi4");
    QString tmpProfile = ptrFile.value("configProfile", "none").toString();
    return tmpProfile;
}

void preferencesDialog::saveValues()
{
    QSettings ptrFile("nmapsi4", "nmapsi4");
    ptrFile.setValue("configProfile", m_ScanActive);
    ptrFile.setValue("logType", comboLogType->currentIndex());
    ptrFile.setValue("hostCache", spinBoxCache->value());

    if (checkWinPos->isChecked())
        ptrFile.setValue("savePos", "true");
    else
        ptrFile.setValue("savePos", "false");

    if (checkSize->isChecked())
        ptrFile.setValue("saveSize", "true");
    else
        ptrFile.setValue("saveSize", "false");

#ifndef Q_WS_WIN
    if (checkBoxlookup->isChecked())
        ptrFile.setValue("lookInternal", "true");
    else
        ptrFile.setValue("lookInternal", "false");

    if (checkBoxDig->isChecked())
        ptrFile.setValue("lookDig", "true");
    else
        ptrFile.setValue("lookDig", "false");
#endif
}


void preferencesDialog::updateItem()
{
    if (m_profileItem->isSelected())
    {
        labelTitle->setText(tr("<h3>Profiles Scan</h3>"));
        stackPref->setCurrentIndex(0);
    }
    else if (m_logItem->isSelected())
    {
        labelTitle->setText(tr("<h3>Automatic Logs Options</h3>"));
        stackPref->setCurrentIndex(1);
    }
    else if (m_sizeItem->isSelected())
    {
        labelTitle->setText(tr("<h3>Size Options</h3>"));
        stackPref->setCurrentIndex(2);
    }
    else if (m_lookItem->isSelected())
    {
        labelTitle->setText(tr("<h3>Scan Lookup</h3>"));
        stackPref->setCurrentIndex(3);
    }
}

void preferencesDialog::setProfile()
{

    int uid = 0;

#ifndef Q_WS_WIN
    uid = getuid();
#endif

    QString tmpProfile = readProfile();

    if (uid)
    {
        groupRoot->setVisible(false);
    }
    else
    {
        checkNormalScan->setEnabled(false);
        checkQuickScan->setEnabled(false);
    }

    if (!tmpProfile.compare("normal"))
    {
        checkNormalScan->setChecked(true);
        m_ScanActive = "normal";
    }

    if (!tmpProfile.compare("quick"))
    {
        checkQuickScan->setChecked(true);
        m_ScanActive = "quick";
    }


    if (!tmpProfile.compare("fullversion") && !uid)
    {
        checkFullVersion->setChecked(true);
        m_ScanActive = "fullversion";
    }
    else if (uid)
    {
        checkFullVersion->setEnabled(false);
    }

    if (!tmpProfile.compare("quickversion") && !uid)
    {
        checkQuickVersion->setChecked(true);
        m_ScanActive = "quickversion";
    }
    else if (uid)
    {
        checkQuickVersion->setEnabled(false);
    }

}

void preferencesDialog::quit()
{
    saveValues(); // save Options
    emit accept();   // send accept signal and exit
}

void preferencesDialog::setDefaults()
{
    checkNormalScan->setChecked(true);
    checkQuickScan->setChecked(false);
    checkFullVersion->setChecked(false);
    checkQuickVersion->setChecked(false);
    checkSize->setChecked(false);
    checkWinPos->setChecked(false);
    checkBoxlookup->setChecked(true);
    checkBoxDig->setChecked(false);
}

void preferencesDialog::updateNormalCheck()   // slot
{
    if (checkNormalScan->isChecked())
    {
        checkQuickScan->setChecked(false);
        checkFullVersion->setChecked(false);
        checkQuickVersion->setChecked(false);
        m_ScanActive = "normal";
    }
}

void preferencesDialog::updateQuickCheck()   //slot
{
    if (checkQuickScan->isChecked())
    {
        checkNormalScan->setChecked(false);
        checkFullVersion->setChecked(false);
        checkQuickVersion->setChecked(false);
        m_ScanActive = "quick";
    }
}

void preferencesDialog::updateFullVersionCheck()   // slot
{
    if (checkFullVersion->isChecked())
    {
        checkNormalScan->setChecked(false);
        checkQuickScan->setChecked(false);
        checkQuickVersion->setChecked(false);
        m_ScanActive = "fullversion";
    }
}

void preferencesDialog::updateQuickVersionCheck()   // slot
{
    if (checkQuickVersion->isChecked())
    {
        checkNormalScan->setChecked(false);
        checkQuickScan->setChecked(false);
        checkFullVersion->setChecked(false);
        m_ScanActive = "quickversion";
    }
}

void preferencesDialog::activeLookupInt()
{
    if(checkBoxlookup->isChecked())
        checkBoxDig->setChecked(false);
}

void preferencesDialog::activeLookupDig()
{
    if(checkBoxDig->isChecked())
        checkBoxlookup->setChecked(false);
}

preferencesDialog::~preferencesDialog()
{
}
