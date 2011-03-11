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


void nmapClass::restoreGlobalProfile() {

    // global profile options
    QSettings settings("nmapsi4", "nmapsi4_gprofile");
    
    // **************** Start Scan section ********************************
    //  user mode 
    rpcBox->setChecked(settings.value("scan/rpc_scan").toBool());
    notpingBox->setChecked(settings.value("scan/dont_ping").toBool());
    versionBox->setChecked(settings.value("scan/version_probe").toBool());
    portCombo->setCurrentIndex(settings.value("scan/port_range").toInt());
    int comboScanIndex_ = settings.value("scan/scan_type").toInt();
    if (!uid) {
	comboScan->setCurrentIndex(comboScanIndex_);
    } else if (comboScanIndex_ < 4) {
	comboScan->setCurrentIndex(comboScanIndex_);
    } else {
	// isn't admin and not valid profile
	comboScan->setCurrentIndex(0);
    }
    portEdit->setText(settings.value("scan/ports_value").toString());
    bounceEdit->setText(settings.value("scan/bounce_host").toString());
    //  admin mode
    if (!uid) {
	checkOS->setChecked(settings.value("scan/os_detection").toBool());
    }
    // **************** End Scan section **********************************
    
    // **************** Start Options section *****************************
    //  user mode
    comboVerbosity->setCurrentIndex(settings.value("options/verbosity_combo").toInt());
    comboDNSResolv->setCurrentIndex(settings.value("options/dnsresolution_combo").toInt());
    checkOrdered->setChecked(settings.value("options/orderdport_checl").toBool());
    checkIpv6->setChecked(settings.value("options/ipv6_check").toBool());
    checkBoxDevice->setChecked(settings.value("options/specificdevice_check").toBool());
    checkSpoof->setChecked(settings.value("options/spoofaddress_check").toBool());
    OlineDevice->setText(settings.value("options/specificdevice_line").toString());
    lineEditSpoof->setText(settings.value("options/spoofaddress_line").toString());
    //  admin mode
    if (!uid) {
	checkFrag->setChecked(settings.value("options/fragmentation").toBool());
	checkDecoy->setChecked(settings.value("options/decoy_check").toBool());
	checkSourcePort->setChecked(settings.value("options/sourceport_check").toBool());
	lineDecoy->setText(settings.value("options/decoy_line").toString());
	lineSourcePort->setText(settings.value("options/sourceport_line").toString());
    }
    // **************** End Options section **********************************
    
    // **************** Start Discover section *****************************
    //  user mode
    checkTcpPing->setChecked(settings.value("discover/tcpack_ping").toBool());
    lineTcpPing->setText(settings.value("discover/tcpack_line").toString());
    checkTcpSyn->setChecked(settings.value("discover/tcpsyn_ping").toBool());
    lineSynPing->setText(settings.value("discover/tcpsyn_line").toString());
    //  admin mode
    if (!uid) {
	checkIcmpTimestamp->setChecked(settings.value("discover/icmp_timestamp").toBool());
	checkIcmpNetmask->setChecked(settings.value("discover/icmp_netmask").toBool());
	checkIcmpEcho->setChecked(settings.value("discover/icmp_echo").toBool());
	checkUdpPing->setChecked(settings.value("discover/udp_ping").toBool());
	lineUdpPing->setText(settings.value("discover/udpping_line").toString());
    }
    // **************** End Discover section **********************************
    
    // **************** Start Timing section *****************************
    //  user mode 
    TcheckIpv4ttl->setChecked(settings.value("timing/ipv4_ttl").toBool());
    spinBoxIpv4ttl->setValue(settings.value("timing/ipv4ttl_spinbox").toInt());
    TcheckMinPar->setChecked(settings.value("timing/min_parallel").toBool());
    TspinBoxMinP->setValue(settings.value("timing/minparallel_spinbox").toInt());
    TcheckMaxPar->setChecked(settings.value("timing/max_parallel").toBool());
    spinBoxMaxPar->setValue(settings.value("timing/maxparallel_spinbox").toInt());
    TcheckInitRtt->setChecked(settings.value("timing/initial_rtt").toBool());
    spinBoxInitRtt->setValue(settings.value("timing/initialrtt_spinbox").toInt());
    TcheckMinRtt->setChecked(settings.value("timing/min_rtt").toBool());
    spinBoxMinRtt->setValue(settings.value("timing/minrtt_spinbox").toInt());
    TcheckMaxRtt->setChecked(settings.value("timing/max_rtt").toBool());
    spinBoxMaxRtt->setValue(settings.value("timing/maxrtt_spinbox").toInt());
    TcheckHostTime->setChecked(settings.value("timing/host_timeout").toBool());
    spinBoxHostTime->setValue(settings.value("timing/hosttime_spinbox").toInt());
    TcheckScanDelay->setChecked(settings.value("timing/scan_delay").toBool());
    spinBoxScanDelay->setValue(settings.value("timing/scandelay_spinbox").toInt());
    comboTiming->setCurrentIndex(settings.value("timing/timing_combo").toInt());
    // **************** End Timing section **********************************

}

void nmapClass::saveGlobalProfile() {

    globalProfile = true;
    
    // global profile options
    QSettings settings("nmapsi4", "nmapsi4_gprofile");
    removeGlobalButton->setEnabled(true);
    
    // **************** Start Scan section ********************************
    //  user mode 
    settings.setValue("scan/rpc_scan", rpcBox->isChecked());
    settings.setValue("scan/dont_ping", notpingBox->isChecked());
    settings.setValue("scan/version_probe", versionBox->isChecked());
    settings.setValue("scan/port_range", portCombo->currentIndex());
    settings.setValue("scan/scan_type", comboScan->currentIndex());
    settings.setValue("scan/ports_value", portEdit->text());
    settings.setValue("scan/bounce_host", bounceEdit->text());
    //  admin mode
    if (!uid) {
	settings.setValue("scan/os_detection", checkOS->isChecked());
    }
    // **************** End Scan section **********************************
    
    // **************** Start Options section *****************************
    //  user mode 
    settings.setValue("options/verbosity_combo", comboVerbosity->currentIndex());
    settings.setValue("options/dnsresolution_combo", comboDNSResolv->currentIndex());
    settings.setValue("options/orderdport_checl", checkOrdered->isChecked());
    settings.setValue("options/ipv6_check", checkIpv6->isChecked());
    settings.setValue("options/specificdevice_check", checkBoxDevice->isChecked());
    settings.setValue("options/spoofaddress_check", checkSpoof->isChecked());
    settings.setValue("options/specificdevice_line", OlineDevice->text());
    settings.setValue("options/spoofaddress_line", lineEditSpoof->text());
    //  admin mode
    if (!uid) {
	settings.setValue("options/fragmentation", checkFrag->isChecked());
	settings.setValue("options/decoy_check", checkDecoy->isChecked());
	settings.setValue("options/sourceport_check", checkSourcePort->isChecked());
	settings.setValue("options/decoy_line", lineDecoy->text());
	settings.setValue("options/sourceport_line", lineSourcePort->text());
    }
    // **************** End Options section **********************************
    
    // **************** Start Discover section *****************************
    //  user mode 
    settings.setValue("discover/tcpack_ping", checkTcpPing->isChecked());
    settings.setValue("discover/tcpack_line", lineTcpPing->text());
    settings.setValue("discover/tcpsyn_ping", checkTcpSyn->isChecked());
    settings.setValue("discover/tcpsyn_line", lineSynPing->text());
    //  admin mode
    if (!uid) {
	settings.setValue("discover/icmp_timestamp", checkIcmpTimestamp->isChecked());
	settings.setValue("discover/icmp_netmask", checkIcmpNetmask->isChecked());
	settings.setValue("discover/icmp_echo", checkIcmpEcho->isChecked());
	settings.setValue("discover/udp_ping", checkUdpPing->isChecked());
	settings.setValue("discover/udpping_line", lineUdpPing->text());
    }
    // **************** End Discover section **********************************
    
    // **************** Start Timing section *****************************
    //  user mode 
    settings.setValue("timing/ipv4_ttl", TcheckIpv4ttl->isChecked());
    settings.setValue("timing/ipv4ttl_spinbox", spinBoxIpv4ttl->value());
    settings.setValue("timing/min_parallel", TcheckMinPar->isChecked());
    settings.setValue("timing/minparallel_spinbox", TspinBoxMinP->value());
    settings.setValue("timing/max_parallel", TcheckMaxPar->isChecked());
    settings.setValue("timing/maxparallel_spinbox", spinBoxMaxPar->value());
    settings.setValue("timing/initial_rtt", TcheckInitRtt->isChecked());
    settings.setValue("timing/initialrtt_spinbox", spinBoxInitRtt->value());
    settings.setValue("timing/min_rtt", TcheckMinRtt->isChecked());
    settings.setValue("timing/minrtt_spinbox", spinBoxMinRtt->value());
    settings.setValue("timing/max_rtt", TcheckMaxRtt->isChecked());
    settings.setValue("timing/maxrtt_spinbox", spinBoxMaxRtt->value());
    settings.setValue("timing/host_timeout", TcheckHostTime->isChecked());
    settings.setValue("timing/hosttime_spinbox", spinBoxHostTime->value());
    settings.setValue("timing/scan_delay", TcheckScanDelay->isChecked());
    settings.setValue("timing/scandelay_spinbox", spinBoxScanDelay->value());
    settings.setValue("timing/timing_combo", comboTiming->currentIndex());
    // **************** End Timing section **********************************
}

void nmapClass::removeGlobalProfile() {
    QSettings settings("nmapsi4", "nmapsi4");
    removeGlobalButton->setEnabled(false);
    globalProfile = false;
    settings.setValue("globalProfile",globalProfile);
    readProfile();
}


