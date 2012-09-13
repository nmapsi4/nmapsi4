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

#include "profilermanager.h"
#include "mainwindow.h"

ProfilerManager::ProfilerManager(MainWindow* parent)
: QDialog(parent), m_ui(parent)
{
    Q_UNUSED(parent);
    initObject();

    setWindowTitle(tr("New profile") + " - Nmapsi4");
}

ProfilerManager::ProfilerManager(const QString profileName, const QString parameters, MainWindow* parent)
: QDialog(parent), m_ui(parent)
{
    Q_UNUSED(parent);
    initObject();
    // restore values
    QStringList parametersList = parameters.split(' ',QString::SkipEmptyParts);

    profileNameLine->setText(profileName);
    m_profiler->restoreValuesFromProfile(parametersList);
    reloadScanParameters();

    setWindowTitle(tr("Edit profile ") + QString("\"") + profileName + QString("\"") + " - Nmapsi4");
}

void ProfilerManager::initObject()
{
    #ifndef Q_WS_WIN
    m_userId = getuid();
#endif

    setupUi(this);

    optionsListScan->setIconSize(QSize(42, 42));
    createQList();

    // create and load nse values from file settings
    m_nseManager = new NseManager(this);
    m_profiler = new Profiler(this);

    connect(optionsListScan, SIGNAL(itemSelectionChanged()),
         this, SLOT(optionListUpdate()));
    connect(portCombo, SIGNAL(activated(QString)),
            this, SLOT(updatePortCombo()));
    //Options
    connect(checkBoxDevice, SIGNAL(toggled(bool)),
            this, SLOT(updateOptions()));
    connect(checkDecoy, SIGNAL(toggled(bool)),
            this, SLOT(updateOptions()));
    connect(checkSpoof, SIGNAL(toggled(bool)),
            this, SLOT(updateOptions()));
    connect(checkSourcePort, SIGNAL(toggled(bool)),
            this, SLOT(updateOptions()));
    connect(comboVerbosity, SIGNAL(activated(QString)),
            this, SLOT(updateComboVerbosity()));
    connect(doneButton, SIGNAL(clicked(bool)),
            this, SLOT(exit()));
    connect(cancelButton, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    connect(comboBaseOptions, SIGNAL(activated(QString)),
            this, SLOT(updateBaseOptions()));

    // nse slots
    connect(nseActiveBut, SIGNAL(clicked()),
            m_nseManager, SLOT(nseTreeActiveItem()));
    connect(nseRemoveBut, SIGNAL(clicked()),
            m_nseManager, SLOT(nseTreeRemoveItem()));
    connect(nseResetBut, SIGNAL(clicked()),
            m_nseManager, SLOT(nseTreeResetItem()));
    connect(nseTreeAvail, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            m_nseManager, SLOT(requestNseHelp(QTreeWidgetItem*,int)));
    connect(nseTreeActive, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            m_nseManager, SLOT(requestNseHelp(QTreeWidgetItem*,int)));
    connect(searchButtHelp, SIGNAL(clicked()),
            m_nseManager, SLOT(requestNseScriptHelp()));

    loadDefaultComboValues();
    loadDefaultBaseProfile();

    m_profileW->setSelected(true);
}

ProfilerManager::~ProfilerManager()
{
    delete m_profiler;
}

void ProfilerManager::createQList()
{
    m_profileW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/document-new.png")),tr("Profile"));
    optionsListScan->addItem(m_profileW);

    m_scanW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/viewmag.png")),tr("Scan"));
    optionsListScan->addItem(m_scanW);

    m_toolW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/tool.png")),tr("Options"));
    optionsListScan->addItem(m_toolW);

    m_discoverW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/network_local.png")),tr("Ping"));
    optionsListScan->addItem(m_discoverW);

    m_timingW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/player_time.png")),tr("Timing"));
    optionsListScan->addItem(m_timingW);

    m_nseW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")),tr("Nse"));
    optionsListScan->addItem(m_nseW);
}

void ProfilerManager::exit()
{
    if (!profileNameLine->text().isEmpty())
    {
        QString parameters(m_profiler->buildExtensions().join(" "));
        qDebug() << "Profiler::Parameters:: " << parameters;

        emit doneParBook(profileNameLine->text(), parameters);
        close();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning - Nmapsi4"), tr("Insert profile name."), tr("Close"));
    }
}

void ProfilerManager::reloadScanParameters()
{
    lineScanParameters->setText(m_profiler->buildExtensions().join(" "));
}

void ProfilerManager::optionListUpdate()
{
    if (m_profileW->isSelected())
    {
        reloadScanParameters();
        stackedOptions->setCurrentIndex(0);
    }
    else if (m_scanW->isSelected())
    {
        stackedOptions->setCurrentIndex(1);
    }
    else if (m_discoverW->isSelected())
    {
        stackedOptions->setCurrentIndex(2);
    }
    else if (m_timingW->isSelected())
    {
        stackedOptions->setCurrentIndex(3);
    }
    else if (m_toolW->isSelected())
    {
        stackedOptions->setCurrentIndex(4);
    }
    else if (m_nseW->isSelected())
    {
        stackedOptions->setCurrentIndex(5);
    }
}

void ProfilerManager::updatePortCombo()
{
    switch (portCombo->currentIndex())
    {
    case 0:
//    Normal
        portEdit->setEnabled(false);
        break;
    case 1:
//    All
        portEdit->setEnabled(false);
        break;
    case 2:
//    Most Important
        portEdit->setEnabled(false);
        break;
    case 3:
//    Range
        portEdit->setEnabled(true);
        break;
    default:
        portEdit->setEnabled(false);
        break;
    }
}

void ProfilerManager::loadDefaultComboValues()
{
    if (!m_userId)
    { // if root
        comboScanTcp->addItem(QApplication::translate("profilerManager", "TCP SYN Stealth Scan (-sS)", 0, QApplication::UnicodeUTF8));
        comboScanTcp->addItem(QApplication::translate("profilerManager", "ACK Stealth Scan (-sA)", 0, QApplication::UnicodeUTF8));
        comboScanTcp->addItem(QApplication::translate("profilerManager", "Mainmon Scan (-sM)", 0, QApplication::UnicodeUTF8));
        comboScanTcp->addItem(QApplication::translate("profilerManager", "FIN Stealth Scan (-sF)", 0, QApplication::UnicodeUTF8));
        comboScanTcp->addItem(QApplication::translate("profilerManager", "NULL Stealth Scan (-sN)", 0, QApplication::UnicodeUTF8));
        comboScanTcp->addItem(QApplication::translate("profilerManager", "XMAS Tree Stealth Scan (-sX)", 0, QApplication::UnicodeUTF8));
        comboScanTcp->addItem(QApplication::translate("profilerManager", "TCP Window Scan (-sW)", 0, QApplication::UnicodeUTF8));

        comboScanNonTcp->addItem(QApplication::translate("profilerManager", "UDP Ports Scan (-sU)", 0, QApplication::UnicodeUTF8));
        comboScanNonTcp->addItem(QApplication::translate("profilerManager", "IP Protocol Scan (-sO)", 0, QApplication::UnicodeUTF8));
        comboScanNonTcp->addItem(QApplication::translate("profilerManager", "SCTP INIT scan (-sY)", 0, QApplication::UnicodeUTF8));
        comboScanNonTcp->addItem(QApplication::translate("profilerManager", "SCTP cookie-echo scan (-sZ)", 0, QApplication::UnicodeUTF8));
    }
    else
    {
        checkIcmpEcho->setVisible(false);
        checkIcmpTimestamp->setVisible(false);
        checkIcmpNetmask->setVisible(false);
        checkOS->setVisible(false);

        // discover Udp Ping
        checkUdpPing->setVisible(false);
        lineUdpPing->setVisible(false);

        // Misc Option
        checkFrag->setVisible(false);
        checkDecoy->setVisible(false);
        lineDecoy->setVisible(false);
        checkSourcePort->setVisible(false);
        lineSourcePort->setVisible(false);

        //idle scan
        checkIdleScan->setVisible(false);
        lineIdleScan->setVisible(false);

        //proto ping
        checkProtoPing->setVisible(false);
        lineProtoPing->setVisible(false);

        //sctp ping
        checkSctpPing->setVisible(false);
        lineSctpPing->setVisible(false);

        //traceroute
        checkTraceroute->setVisible(false);
    }
}

void ProfilerManager::updateOptions()
{
    if (checkSpoof->isChecked())
    {
        comboScanTcp->setCurrentIndex(1);
    }

    if (checkSourcePort->isChecked())
    {
        comboScanTcp->setCurrentIndex(3);
    }
}

void ProfilerManager::updateComboVerbosity()
{
    if (comboVerbosity->currentIndex() == 4)
        QMessageBox::warning(this, "NmapSI4", tr("Warning: Operation more expansive.\n"), tr("Close"));
}

void ProfilerManager::loadDefaultBaseProfile()
{
    QListIterator< QPair<QString, QString> > i(m_ui->defaultScanProfile());
    while (i.hasNext())
    {
        comboBaseOptions->insertItem(comboBaseOptions->count()+1, i.next().first);
    }
}

void ProfilerManager::updateBaseOptions()
{
    if (!comboBaseOptions->currentIndex())
    {
        m_profiler->resetOptions();
        reloadScanParameters();
        return;
    }

    QString parameters;
    QListIterator< QPair<QString, QString> > i(m_ui->defaultScanProfile());
    while (i.hasNext())
    {
        QPair<QString, QString> profile = i.next();
        if (profile.first == comboBaseOptions->currentText())
        {
            parameters =  profile.second;
            break;
        }
    }

    // NOTE: if is not necessary
    if (!parameters.isEmpty())
    {
        m_profiler->resetOptions();
        m_profiler->restoreValuesFromProfile(parameters.split(' '));
    }

    reloadScanParameters();
}
