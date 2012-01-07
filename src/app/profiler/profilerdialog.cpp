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


#include "profilerdialog.h"

profilerManager::profilerManager(QWidget* parent) : QDialog(parent)
{
    Q_UNUSED(parent);

#ifndef Q_WS_WIN
    m_uid = getuid();
#endif

    setupUi(this);

    optionsListScan->setIconSize(QSize(52, 52));
    createQList();

    connect(optionsListScan, SIGNAL(itemSelectionChanged()),
         this, SLOT(optionListUpdate()));
    connect(portCombo, SIGNAL(activated(QString)),
            this, SLOT(update_portCombo()));  // portCombo slot
    // discover
    connect(checkTcpPing, SIGNAL(toggled(bool)),
            this, SLOT(update_discover()));  // discover check
    connect(checkTcpSyn, SIGNAL(toggled(bool)),
            this, SLOT(update_discover()));  // discover check
    connect(checkUdpPing, SIGNAL(toggled(bool)),
            this, SLOT(update_discover()));  // discover check

    // Timing Slot (Enable and Disable spinBox)
    connect(TcheckIpv4ttl, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckMinPar, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckMaxPar, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckInitRtt, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckMinRtt, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckMaxRtt, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckHostTime, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));
    connect(TcheckScanDelay, SIGNAL(toggled(bool)),
            this, SLOT(update_timing()));

    //Options
    connect(checkBoxDevice, SIGNAL(toggled(bool)),
            this, SLOT(update_options()));
    connect(checkDecoy, SIGNAL(toggled(bool)),
            this, SLOT(update_options()));
    connect(checkSpoof, SIGNAL(toggled(bool)),
            this, SLOT(update_options()));
    connect(checkSourcePort, SIGNAL(toggled(bool)),
            this, SLOT(update_options()));
    connect(comboVerbosity, SIGNAL(activated(QString)),
            this, SLOT(update_comboVerbosity()));

    connect(doneButton, SIGNAL(clicked(bool)),
            this, SLOT(exit()));
    connect(cancelButton, SIGNAL(clicked(bool)),
            this, SLOT(close()));


    loadDefaultBaseParameters();
    loadDefaultComboValues();
}


profilerManager::~profilerManager()
{
    delete m_timingW;
    delete m_discoverW;
    delete m_toolW;
    delete m_scanW;
}

void profilerManager::createQList()
{
    m_scanW = new QListWidgetItem(optionsListScan);
    m_scanW->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    m_scanW->setText(tr("Scan")); // scan Options
    m_scanW->setSelected(true);

    m_toolW = new QListWidgetItem(optionsListScan);
    m_toolW->setIcon(QIcon(QString::fromUtf8(":/images/images/tool.png")));
    m_toolW->setText(tr("Options"));

    m_discoverW = new QListWidgetItem(optionsListScan);
    m_discoverW->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    m_discoverW->setText(tr("Discover"));

    m_timingW = new QListWidgetItem(optionsListScan);
    m_timingW->setIcon(QIcon(QString::fromUtf8(":/images/images/player_time.png")));
    m_timingW->setText(tr("Timing"));
}

void profilerManager::exit()
{
    if (!profileNameLine->text().isEmpty())
    {
        QString parameters(buildExtensions().join(" "));
        qDebug() << "Profiler::Parameters:: " << parameters;

        //emit doneParBook(profileNameLine->text(), parameters);
        close();
    }
    else
    {
        // FIXME: show message for empty name
        qWarning() << "Please enter profile name";
    }
}

void profilerManager::optionListUpdate()
{
    if (m_scanW->isSelected())
    {
        stackedOptions->setCurrentIndex(0);
    }
    else if (m_discoverW->isSelected())
    {
        stackedOptions->setCurrentIndex(1);
    }
    else if (m_timingW->isSelected())
    {
        stackedOptions->setCurrentIndex(2);
    }
    else if (m_toolW->isSelected())
    {
        stackedOptions->setCurrentIndex(3);
    }
}

QStringList profilerManager::buildExtensions()
{
    QStringList parameters;

    switch (comboScanTcp->currentIndex()) { //scan check
    case 1:
//    Connect Scan
        parameters << "-sT";
        break;
    case 2:
//    Ping Sweep
        parameters << "-sP";
        break;
    case 3:
//    SYN Stealth Scan (rootMode)
        parameters << "-sS";
        break;
    case 4:
//    ACK Stealth Scan (rootMode)
        parameters << "-sA";
        break;
    case 5:
//    Mainmon Stealth Scan (rootMode)
        parameters << "-sM";
        break;
    case 6:
//    FIN Stealth Scan (rootMode)
        parameters << "-sF";
        break;
    case 7:
//    NULL Stealth Scan (rootMode)
        parameters << "-sN";
        break;
    case 8:
//    XMAS Tree Stealth Scan (rootMode)
        parameters << "-sX";
        break;
    case 9:
//    TCP Window Scan (rootMode)
        parameters << "-sW";
        break;
    default:
        comboScanTcp->setCurrentIndex(1);
        parameters << "-sT";
        break;

    } // end switch scan

    switch(comboScanNonTcp->currentIndex())
    {
    case 1:
        //Host List
        parameters << "-sL";
        break;
    case 2:
        //UDP Ports Scan (rootMode)
        parameters << "-sU";
        break;
    case 3:
        //IP Protocol Scan (rootMode)
        parameters << "-sO";
        break;
    case 4:
        //SCTP INIT port scan (rootMode)
        parameters << "-sY";
        break;
    case 5:
        //SCTP cookie-echo port scan (rootMode)
        parameters << "-sZ";
        break;
    }

    if (checkFtpBounce->isChecked() && !bounceEdit->text().isEmpty())
    {
        // FTP Bounce attack
        parameters << "-b";
        parameters << bounceEdit->text();
    }
    else
    {
        checkFtpBounce->setCheckState(Qt::Unchecked);
    }

    if (checkIdleScan->isChecked() && !lineIdleScan->text().isEmpty())
    {
        // Idle scan
        parameters << "-sI";
        parameters << lineIdleScan->text();
    }
    else
    {
        checkIdleScan->setCheckState(Qt::Unchecked);
    }

    // start option scan
    if (rpcBox->isChecked())
    {
        parameters << "-sR";
    }

    if (versionBox->isChecked())
    {
        parameters << "-sV";
    }

    if (notpingBox->isChecked())
    {
        parameters << "-P0";
    }

    if (checkOS->isChecked())
    {
        parameters << "-O";
    }
    //end Extension

    switch (portCombo->currentIndex())
    { // port combo check
    case 1:
//    All
        parameters << "-p-";
        break;
    case 2:
//    Most Important
        parameters << "-F";
        break;
    case 3:
//    Range
        if (!portEdit->text().isEmpty())
        {
            parameters << "-p";
            parameters << portEdit->text();
        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", tr("No Ports Range (ex: 20-80)\n"), tr("Close"));
        }
    default:
        break;
    }


    if (checkTcpPing->isChecked() && !lineTcpPing->text().isEmpty())
    { // Discover options (tcp ack)
        QString tmpCommand;
        tmpCommand.append("-PT");
        tmpCommand.append(lineTcpPing->text());
        parameters << tmpCommand;
    }
    else
    {
        checkTcpPing->setCheckState(Qt::Unchecked);
    }

    if (checkTcpSyn->isChecked() && !lineSynPing->text().isEmpty())
    { // Discover options (tcp syn)

        QString tmpCommand;
        tmpCommand.append("-PS");
        tmpCommand.append(lineSynPing->text());
        parameters << tmpCommand;
    }
    else
    {
        checkTcpSyn->setCheckState(Qt::Unchecked);
    }

    if (checkUdpPing->isChecked() && !lineUdpPing->text().isEmpty())
    { // Discover options (tcp syn)
        QString tmpCommand;
        tmpCommand.append("-PU");
        tmpCommand.append(lineUdpPing->text());
        parameters << tmpCommand;
    }
    else
    {
        checkUdpPing->setCheckState(Qt::Unchecked);
    }

    // Discover option
    if (checkIcmpEcho->isChecked())
    {
        parameters << "-PI";
    }

    if (checkIcmpTimestamp->isChecked())
    {
        parameters << "-PP";
    }

    if (checkIcmpNetmask->isChecked())
    {
        parameters << "-PM";
    }

    switch (comboTiming->currentIndex())
    { // port combo Timing
    case 1:
        //Paranoid
        parameters << "-T0";
        break;
    case 2:
        //Sneaky
        parameters << "-T1";
        break;
    case 3:
        //Polite
        parameters << "-T2";
        break;
    case 4:
        //Normal
        parameters << "-T3";
        break;
    case 5:
        //Aggressive
        parameters << "-T4";
        break;
    case 6:
        //Insane
        parameters << "-T5";
        break;
    default:
        break;
    }

    switch (comboDNSResolv->currentIndex())
    { // port DNS resolv
    case 1:
//    Always
        parameters << "-R";
        break;
    case 2:
//    Never
        parameters << "-n";
        break;
    default:
        break;
    }

    switch (comboVerbosity->currentIndex())
    { // port DNS resolv
    case 1:
//    Verbose
        parameters << "-v";
        break;
    case 2:
//    Very Verbose
        parameters << "-vv";
        break;
    case 3:
//    Debug
        parameters << "-d";
        break;
    case 4:
//    Verbose Debug
        parameters << "-d2";
        break;
    default:
        break;
    }

    // Misc Options
    if (checkOrdered->isChecked())
    {
        parameters << "-r"; // Ordered Port
    }

    if (checkIpv6->isChecked())
    {
        parameters << "-6"; // Ipv6
    }

    if (checkFrag->isChecked())
    {
        parameters << "-f"; // Ipv6
    }

    // Timing options
    if (TcheckIpv4ttl->isChecked())
    {
        parameters << "--ttl";
        parameters << spinBoxIpv4ttl->text();
    }

    if (TcheckMinPar->isChecked())
    {
        parameters << "--min_parallelism";
        parameters << TspinBoxMinP->text();
    }

    if (TcheckMaxPar->isChecked())
    {
        parameters << "--max_parallelism";
        parameters << spinBoxMaxPar->text();
    }

    if (TcheckInitRtt->isChecked())
    {
        parameters << "--initial_rtt_timeout";
        parameters << spinBoxInitRtt->text();
    }

    if (TcheckMinRtt->isChecked())
    {
        parameters << "--min_rtt_timeout";
        parameters << spinBoxMinRtt->text();
    }

    if (TcheckMaxRtt->isChecked())
    {
        parameters << "--max_rtt_timeout";
        parameters << spinBoxMaxRtt->text();
    }

    if (TcheckHostTime->isChecked())
    {
        parameters << "--host_timeout";
        parameters << spinBoxHostTime->text();
    }

    if (TcheckScanDelay->isChecked())
    {
        parameters << "--scan_delay";
        parameters << spinBoxScanDelay->text();
    }

    //Options
    if (checkBoxDevice->isChecked() && !OlineDevice->text().isEmpty())
    { // Discover options (tcp syn)
            parameters << "-e";
            parameters << OlineDevice->text();
    }
    else
    {
        checkBoxDevice->setCheckState(Qt::Unchecked);
    }

    if (checkDecoy->isChecked())
    { // Discover options (tcp syn)
        if (!lineDecoy->text().isEmpty())
        {
            parameters << "-D";
            parameters << lineDecoy->text();

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "Please, first insert a Decoy\n", "Disable Option");
            checkDecoy->setCheckState(Qt::Unchecked);
        }
    }

    if (checkSpoof->isChecked())
    { // Spoof options
        if (!lineEditSpoof->text().isEmpty())
        {
            parameters << "-S";
            parameters << lineEditSpoof->text();

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "Please, insert spoof address\n", "Disable Option");
            checkSpoof->setCheckState(Qt::Unchecked);
        }
    }

    if (checkSourcePort->isChecked())
    { // Spoof options
        if (!lineSourcePort->text().isEmpty())
        {
            parameters << "-g";
            parameters << lineSourcePort->text();

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "Please, insert port address\n", "Disable Option");
            checkSourcePort->setCheckState(Qt::Unchecked);
        }
    }

    return parameters;
}

void profilerManager::update_portCombo()
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

void profilerManager::loadDefaultComboValues()
{
    if (!m_uid)
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

        comboScanTcp->setCurrentIndex(3);

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
        label_7->setVisible(false);

        // Misc Option
        checkFrag->setVisible(false);
        checkDecoy->setVisible(false);
        lineDecoy->setVisible(false);
        checkSourcePort->setVisible(false);
        lineSourcePort->setVisible(false);

        //idle scan
        checkIdleScan->setVisible(false);
        lineIdleScan->setVisible(false);
    }
}

void profilerManager::update_discover()
{
    if (checkTcpPing->isChecked())
    {
        lineTcpPing->setEnabled(true);
        lineTcpPing->clear();
    }
    else
    {
        lineTcpPing->setEnabled(false);
        lineTcpPing->clear();
    }

    if (checkTcpSyn->isChecked())
    {
        lineSynPing->setEnabled(true);
        lineSynPing->clear();
    }
    else
    {
        lineSynPing->setEnabled(false);
        lineSynPing->clear();
    }

    if (checkUdpPing->isChecked())
    {
        lineUdpPing->setEnabled(true);
        lineUdpPing->clear();
    }
    else
    {
        lineUdpPing->setEnabled(false);
        lineUdpPing->clear();
    }
}

void profilerManager::update_timing()
{
    if (TcheckIpv4ttl->isChecked())
        spinBoxIpv4ttl->setEnabled(true);
    else
        spinBoxIpv4ttl->setEnabled(false);

    if (TcheckMinPar->isChecked())
        TspinBoxMinP->setEnabled(true);
    else
        TspinBoxMinP->setEnabled(false);

    if (TcheckMaxPar->isChecked())
        spinBoxMaxPar->setEnabled(true);
    else
        spinBoxMaxPar->setEnabled(false);

    if (TcheckInitRtt->isChecked())
        spinBoxInitRtt->setEnabled(true);
    else
        spinBoxInitRtt->setEnabled(false);

    if (TcheckMinRtt->isChecked())
        spinBoxMinRtt->setEnabled(true);
    else
        spinBoxMinRtt->setEnabled(false);

    if (TcheckMaxRtt->isChecked())
        spinBoxMaxRtt->setEnabled(true);
    else
        spinBoxMaxRtt->setEnabled(false);

    if (TcheckHostTime->isChecked())
        spinBoxHostTime->setEnabled(true);
    else
        spinBoxHostTime->setEnabled(false);

    if (TcheckScanDelay->isChecked())
        spinBoxScanDelay->setEnabled(true);
    else
        spinBoxScanDelay->setEnabled(false);
}

void profilerManager::update_options()
{
    if (checkBoxDevice->isChecked())
    {
        OlineDevice->setEnabled(true);
    }
    else
    {
        OlineDevice->setEnabled(false);
        OlineDevice->clear();
    }

    if (checkDecoy->isChecked())
    {
        lineDecoy->setEnabled(true);
    }
    else
    {
        lineDecoy->setEnabled(false);
        lineDecoy->clear();
    }

    if (checkSpoof->isChecked())
    {
        lineEditSpoof->setEnabled(true);
        comboScanTcp->setCurrentIndex(1);
        notpingBox->setChecked(true);
        checkBoxDevice->setChecked(true);
    }
    else
    {
        lineEditSpoof->setEnabled(false);
        lineEditSpoof->clear();
        checkBoxDevice->setCheckState(Qt::Unchecked);
        notpingBox->setChecked(false);
    }

    if (checkSourcePort->isChecked())
    {
        lineSourcePort->setEnabled(true);
        comboScanTcp->setCurrentIndex(3);
    }
    else
    {
        lineSourcePort->setEnabled(false);
        lineSourcePort->clear();
    }
}

void profilerManager::update_comboVerbosity()
{
    if (comboVerbosity->currentIndex() == 4)
        QMessageBox::warning(this, "NmapSI4", tr("Warning: Operation more expansive.\n"), tr("Close"));
}

void profilerManager::loadDefaultBaseParameters()
{
    if (!m_uid)
    {
        setFullVersionProfile();
    }
    else
    {
        setNormalProfile();
    }
}

void profilerManager::setNormalProfile()
{
    resetOptions();
    comboTiming->setCurrentIndex(4);
    comboDNSResolv->setCurrentIndex(0);
    comboVerbosity->setCurrentIndex(1);
    versionBox->setChecked(true);
}

void profilerManager::setFullVersionProfile()
{
    resetOptions();
    comboTiming->setCurrentIndex(4);
    checkOS->setChecked(true);
    versionBox->setChecked(true);
    comboVerbosity->setCurrentIndex(1);
    comboDNSResolv->setCurrentIndex(0);
}

void profilerManager::resetOptions()
{
    versionBox->setChecked(false);
    checkOS->setChecked(false);
    comboTiming->setCurrentIndex(0);
    comboDNSResolv->setCurrentIndex(0);
    comboVerbosity->setCurrentIndex(0);
}

