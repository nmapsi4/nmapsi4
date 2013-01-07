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

#include "profilermanager.h"
#include "mainwindow.h"

ProfilerManager::ProfilerManager(MainWindow* parent)
    : QDialog(parent), m_dialogUi(new Ui::profilerDialog), m_ui(parent)
{
    Q_UNUSED(parent);
    initObject();

    setWindowTitle(tr("New profile") + " - Nmapsi4");
}

ProfilerManager::ProfilerManager(const QString profileName, const QString parameters, MainWindow* parent)
    : QDialog(parent), m_dialogUi(new Ui::profilerDialog), m_ui(parent)
{
    Q_UNUSED(parent);
    initObject();
    // restore values
    QStringList parametersList = parameters.split(' ', QString::SkipEmptyParts);

    m_dialogUi->profileNameLine->setText(profileName);
    m_profiler->restoreValuesFromProfile(parametersList);
    reloadScanParameters();

    setWindowTitle(tr("Edit profile ") + QString("\"") + profileName + QString("\"") + " - Nmapsi4");
    // disable combo base option
    m_dialogUi->comboBaseOptions->setVisible(false);
    m_dialogUi->baseLabelCombo->setVisible(false);
}

void ProfilerManager::initObject()
{
#ifndef Q_WS_WIN
    m_userId = getuid();
#endif

    m_dialogUi->setupUi(this);

    m_dialogUi->optionsListScan->setIconSize(QSize(42, 42));
    createQList();

    // create and load nse values from file settings
    m_nseManager = new NseManager(this);
    m_profiler = new Profiler(this);

    connect(m_dialogUi->optionsListScan, SIGNAL(itemSelectionChanged()),
            this, SLOT(optionListUpdate()));
    connect(m_dialogUi->portCombo, SIGNAL(activated(QString)),
            this, SLOT(updatePortCombo()));
    //Options
    connect(m_dialogUi->checkBoxDevice, SIGNAL(toggled(bool)),
            this, SLOT(updateOptions()));
    connect(m_dialogUi->checkDecoy, SIGNAL(toggled(bool)),
            this, SLOT(updateOptions()));
    connect(m_dialogUi->checkSpoof, SIGNAL(toggled(bool)),
            this, SLOT(updateOptions()));
    connect(m_dialogUi->checkSourcePort, SIGNAL(toggled(bool)),
            this, SLOT(updateOptions()));
    connect(m_dialogUi->comboVerbosity, SIGNAL(activated(QString)),
            this, SLOT(updateComboVerbosity()));
    connect(m_dialogUi->doneButton, SIGNAL(clicked(bool)),
            this, SLOT(exit()));
    connect(m_dialogUi->pushButtonQuickProfile, SIGNAL(clicked(bool)),
            this, SLOT(exitWithQuickProfile()));
    connect(m_dialogUi->cancelButton, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    connect(m_dialogUi->comboBaseOptions, SIGNAL(activated(QString)),
            this, SLOT(updateBaseOptions()));

    // nse slots
    connect(m_dialogUi->nseActiveBut, SIGNAL(clicked()),
            m_nseManager, SLOT(nseTreeActiveItem()));
    connect(m_dialogUi->nseRemoveBut, SIGNAL(clicked()),
            m_nseManager, SLOT(nseTreeRemoveItem()));
    connect(m_dialogUi->nseResetBut, SIGNAL(clicked()),
            m_nseManager, SLOT(nseTreeResetItem()));
    connect(m_dialogUi->nseTreeAvail, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            m_nseManager, SLOT(requestNseHelp(QTreeWidgetItem*,int)));
    connect(m_dialogUi->nseTreeActive, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            m_nseManager, SLOT(requestNseHelp(QTreeWidgetItem*,int)));
    connect(m_dialogUi->searchButtHelp, SIGNAL(clicked()),
            m_nseManager, SLOT(requestNseScriptHelp()));

    loadDefaultComboValues();
    loadDefaultBaseProfile();

    m_profileW->setSelected(true);
}

ProfilerManager::~ProfilerManager()
{
    delete m_profiler;
    delete m_dialogUi;
}

void ProfilerManager::createQList()
{
    m_profileW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/document-new.png")), tr("Profile"));
    m_dialogUi->optionsListScan->addItem(m_profileW);

    m_scanW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/viewmag.png")), tr("Scan"));
    m_dialogUi->optionsListScan->addItem(m_scanW);

    m_targetW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/Categories-applications-development-web-icon.png")), tr("Target"));
    m_dialogUi->optionsListScan->addItem(m_targetW);

    m_toolW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/tool.png")), tr("Other"));
    m_dialogUi->optionsListScan->addItem(m_toolW);

    m_discoverW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/network_local.png")), tr("Ping"));
    m_dialogUi->optionsListScan->addItem(m_discoverW);

    m_timingW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/player_time.png")), tr("Timing"));
    m_dialogUi->optionsListScan->addItem(m_timingW);

    m_nseW = new QListWidgetItem(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")), tr("Nse"));
    m_dialogUi->optionsListScan->addItem(m_nseW);
}

void ProfilerManager::exit()
{
    QString parameters(m_profiler->buildExtensions().join(" "));
    qDebug() << "Profiler::Parameters:: " << parameters;

    if (!m_dialogUi->profileNameLine->text().isEmpty() && !parameters.isEmpty()) {
        emit doneParBook(m_dialogUi->profileNameLine->text(), parameters);
        close();
    } else {
        QMessageBox::warning(this, tr("Warning - Nmapsi4"), tr("Insert profile name or selected the options."), tr("Close"));
    }
}

void ProfilerManager::exitWithQuickProfile()
{
    QStringList parameters(m_profiler->buildExtensions());

    if (!parameters.isEmpty()) {
        emit doneQuickProfile(parameters);
        close();
    } else {
        QMessageBox::warning(this, tr("Warning - Nmapsi4"), tr("No option selected."), tr("Close"));
    }

}

void ProfilerManager::reloadScanParameters()
{
    m_dialogUi->lineScanParameters->setText(m_profiler->buildExtensions().join(" "));
}

void ProfilerManager::optionListUpdate()
{
    if (m_profileW->isSelected()) {
        reloadScanParameters();
        m_dialogUi->stackedOptions->setCurrentIndex(0);
    } else if (m_scanW->isSelected()) {
        m_dialogUi->stackedOptions->setCurrentIndex(1);
    } else if (m_discoverW->isSelected()) {
        m_dialogUi->stackedOptions->setCurrentIndex(2);
    } else if (m_timingW->isSelected()) {
        m_dialogUi->stackedOptions->setCurrentIndex(3);
    } else if (m_toolW->isSelected()) {
        m_dialogUi->stackedOptions->setCurrentIndex(4);
    } else if (m_nseW->isSelected()) {
        m_dialogUi->stackedOptions->setCurrentIndex(5);
    } else if (m_targetW->isSelected()) {
        m_dialogUi->stackedOptions->setCurrentIndex(6);
    }
}

void ProfilerManager::updatePortCombo()
{
    switch (m_dialogUi->portCombo->currentIndex()) {
    case 0:
//    Normal
        m_dialogUi->portEdit->setEnabled(false);
        break;
    case 1:
//    All
        m_dialogUi->portEdit->setEnabled(false);
        break;
    case 2:
//    Range
        m_dialogUi->portEdit->setEnabled(true);
        break;
    default:
        m_dialogUi->portEdit->setEnabled(false);
        break;
    }
}

void ProfilerManager::loadDefaultComboValues()
{
    if (!m_userId) {
        // if root
        m_dialogUi->comboScanTcp->addItem(tr("TCP SYN Stealth Scan (-sS)"));
        m_dialogUi->comboScanTcp->addItem(tr("ACK Stealth Scan (-sA)"));
        m_dialogUi->comboScanTcp->addItem(tr("Mainmon Scan (-sM)"));
        m_dialogUi->comboScanTcp->addItem(tr("FIN Stealth Scan (-sF)"));
        m_dialogUi->comboScanTcp->addItem(tr("NULL Stealth Scan (-sN)"));
        m_dialogUi->comboScanTcp->addItem(tr("XMAS Tree Stealth Scan (-sX)"));
        m_dialogUi->comboScanTcp->addItem(tr("TCP Window Scan (-sW)"));

        m_dialogUi->comboScanNonTcp->addItem(tr("UDP Ports Scan (-sU)"));
        m_dialogUi->comboScanNonTcp->addItem(tr("IP Protocol Scan (-sO)"));
        m_dialogUi->comboScanNonTcp->addItem(tr("SCTP INIT scan (-sY)"));
        m_dialogUi->comboScanNonTcp->addItem(tr("SCTP cookie-echo scan (-sZ)"));
    } else {
        m_dialogUi->checkIcmpEcho->setVisible(false);
        m_dialogUi->checkIcmpTimestamp->setVisible(false);
        m_dialogUi->checkIcmpNetmask->setVisible(false);
        m_dialogUi->checkOS->setVisible(false);

        // discover Udp Ping
        m_dialogUi->checkUdpPing->setVisible(false);
        m_dialogUi->lineUdpPing->setVisible(false);

        // Misc Option
        m_dialogUi->checkFrag->setVisible(false);
        m_dialogUi->checkDecoy->setVisible(false);
        m_dialogUi->lineDecoy->setVisible(false);
        m_dialogUi->checkSourcePort->setVisible(false);
        m_dialogUi->lineSourcePort->setVisible(false);

        //idle scan
        m_dialogUi->checkIdleScan->setVisible(false);
        m_dialogUi->lineIdleScan->setVisible(false);

        //proto ping
        m_dialogUi->checkProtoPing->setVisible(false);
        m_dialogUi->lineProtoPing->setVisible(false);

        //sctp ping
        m_dialogUi->checkSctpPing->setVisible(false);
        m_dialogUi->lineSctpPing->setVisible(false);

        //traceroute
        m_dialogUi->checkTraceroute->setVisible(false);
    }
}

void ProfilerManager::updateOptions()
{
    if (m_dialogUi->checkSpoof->isChecked()) {
        m_dialogUi->comboScanTcp->setCurrentIndex(1);
    }

    if (m_dialogUi->checkSourcePort->isChecked()) {
        m_dialogUi->comboScanTcp->setCurrentIndex(3);
    }
}

void ProfilerManager::updateComboVerbosity()
{
    if (m_dialogUi->comboVerbosity->currentIndex() == 4) {
        QMessageBox::warning(this, "NmapSI4", tr("Warning: Operation more expansive.\n"), tr("Close"));
    }
}

void ProfilerManager::loadDefaultBaseProfile()
{
    QListIterator< QPair<QString, QString> > i(m_ui->m_profileHandler->defaultScanProfile());
    while (i.hasNext()) {
        m_dialogUi->comboBaseOptions->insertItem(m_dialogUi->comboBaseOptions->count() + 1, i.next().first);
    }
}

void ProfilerManager::updateBaseOptions()
{
    if (!m_dialogUi->comboBaseOptions->currentIndex()) {
        m_profiler->resetOptions();
        reloadScanParameters();
        return;
    }

    QString parameters;
    QListIterator< QPair<QString, QString> > i(m_ui->m_profileHandler->defaultScanProfile());
    while (i.hasNext()) {
        QPair<QString, QString> profile = i.next();
        if (profile.first == m_dialogUi->comboBaseOptions->currentText()) {
            parameters =  profile.second;
            break;
        }
    }

    // NOTE: if is not necessary
    if (!parameters.isEmpty()) {
        m_profiler->resetOptions();
        m_profiler->restoreValuesFromProfile(parameters.split(' '));
    }

    reloadScanParameters();
}
