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

#include "mainwin.h"

parserObj* nmapClass::parser(const QStringList parList, QString StdoutStr, 
                       QString StderrorStr, QTreeWidgetItem* mainTreeE)
{
    /*
     * _logFilePath and verboseLog is global in Ui::MainWindow
     * checkViewOS() methos in parserUtils
     * TODO:: remove _logFilePath and verboleLog with porting to new save log.
     */
    // Create parser Obect
    parserObj *elemObj = new parserObj();
    QString hostCheck = parList[parList.size()-1];
    elemObj->setHostName(hostCheck);
    
    QRegExp rxT_("^\\d\\d?");
    QString generalBuffer_(hostCheck);
    QString tmp;
    QString scanBuffer;
    QString bufferInfo;
    QString bufferTraceroot;
    QString bufferNSS;

    QTextStream stream(&StdoutStr);
    
    while (!stream.atEnd()) 
    {
        tmp = stream.readLine();

        if ((rxT_.indexIn(tmp) != -1) &&
                (tmp.contains("open")
                || tmp.contains("closed")
                || tmp.contains("filtered")
                || tmp.contains("unfiltered"))
                && !tmp.contains("Not shown:")
                && !tmp.contains("Discovered")) 
        {

            scanBuffer.append(tmp);
            scanBuffer.append("\n");
        }

        if ((tmp.contains("MAC")
                || tmp.contains("Running:")
                || tmp.contains("Running")
                || tmp.contains("OS details:")
                || tmp.contains("Aggressive OS guesses:")
                || tmp.contains("Device type:")
                || tmp.contains("Uptime:")
                || tmp.contains("TCP Sequence Prediction:")
                || tmp.contains("IPID Sequence Generation:")
                || tmp.contains("IP ID Sequence Generation:")
                || (tmp.contains("Service Info:") && tmp.compare(generalBuffer_))
                || tmp.contains("Initiating Ping ")
                || tmp.contains("Completed Ping ")
                || tmp.contains("Network Distance:")
                || tmp.contains("Note:")
                || tmp.contains("Nmap done:")
                || tmp.startsWith(QLatin1String("Hosts"))
                || tmp.startsWith(QLatin1String("Host")))
                && !tmp.startsWith(QLatin1String("|"))) 
        {
            bufferInfo.append(tmp);
            bufferInfo.append("\n");
        }
          
        // pars for subtree service
        if(tmp.startsWith(QLatin1String("|"))) 
        {
            QString tmpClean(tmp);
            if(tmpClean.startsWith(QLatin1String("|"))) 
            {
                tmpClean.remove('|');
            }
            
            if(tmpClean.startsWith(QLatin1String("_"))) 
            {
                tmpClean.remove('_');
            }

            int pos;
            while(tmpClean.startsWith(QLatin1String(" "))) 
            {
                pos = tmpClean.indexOf(" ");
                if(pos == 0) 
                {
                    tmpClean.remove(pos,1);
                }
            }

            bufferNSS.append(tmpClean);
            bufferNSS.append("\n");
        }

        if((rxT_.indexIn(tmp) != -1) && (!tmp.contains("/"))) 
        {
            bufferTraceroot.append(tmp);
            bufferTraceroot.append("\n");
        }

    }
    
    // check for log file
    QTextStream *file = NULL;
    if (_logFilePath) 
    {
        file = new QTextStream(_logFilePath);
        // parameters list for log
        QString nmap_command(QString("\n==LogStart: ") + QString("\nnmap ") + parList.join(" "));
        *file << nmap_command << endl << endl;
    }
    
    QString tmp_host;

    if (!generalBuffer_.isEmpty()) 
    {
        //QFont rootFont = root->font(0);
        //rootFont.setWeight(QFont::Normal);
        tmp_host.append(generalBuffer_ + "\n" + QDateTime::currentDateTime().toString("ddd MMM d yy - hh:mm:ss"));
        mainTreeE->setText(0, tmp_host);
        
        if ((_logFilePath) && (!verboseLog))
        {
            *file << generalBuffer_ << endl;
        }
    } 
    else 
    {
        tmp_host.append(hostCheck + "\n" + QDateTime::currentDateTime().toString("ddd MMM d yy - hh:mm:ss")); 
        mainTreeE->setText(0, tmp_host);
        
        if ((_logFilePath) && (!verboseLog))
        {
            *file << hostCheck << endl;
        }
    }
    
    QTextStream scanBufferToStream_(&scanBuffer); // QString to QtextStream (scan Tree)
    QString scanBufferToStream_line;

    // check for scan result
    if (!scanBufferToStream_.atEnd()) 
    {
        while (!scanBufferToStream_.atEnd()) 
        {
            scanBufferToStream_line = scanBufferToStream_.readLine();
            if (scanBufferToStream_line.contains("open") || scanBufferToStream_line.contains("filtered")
                    || scanBufferToStream_line.contains("unfiltered")) 
            {

                if (scanBufferToStream_line.contains("filtered") || scanBufferToStream_line.contains("unfiltered")) 
                {
                    elemObj->setPortFiltered(scanBufferToStream_line);
                } 
                else 
                {
                    elemObj->setPortOpen(scanBufferToStream_line);
                }
            } 
            else 
            {
                elemObj->setPortClose(scanBufferToStream_line);
            }

            if (!scanBufferToStream_line.isEmpty()) 
            {
                QString tmpStr = scanBufferToStream_line;
                QStringList lStr = tmpStr.split(' ', QString::SkipEmptyParts);
                elemObj->setServices(lStr[2]); // Obj Services
                elemObj->setPortServices(lStr[0]);

                if ((_logFilePath) && (!verboseLog)) 
                {
                    *file << scanBufferToStream_line << endl;
                }
            }
        } // end while
    } 
    else 
    {
        mainTreeE->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    QTextStream bufferInfoStream(&bufferInfo); // QString to QtextStrem (scan Tree)
    QString bufferInfoStream_line;

    // check for Host information
    // OS not detected
    bool state_ = false;
    while (!bufferInfoStream.atEnd()) 
    {
         bufferInfoStream_line = bufferInfoStream.readLine();
         // Check OS String
         if (bufferInfoStream_line.contains("OS") && !state_) 
         {
            // OS was found ?
            //state_ = checkViewOS(bufferInfoStream_line,mainTreeE);
            state_ = hostTools::checkViewOS(bufferInfoStream_line,mainTreeE);
         }

         elemObj->setMainInfo(bufferInfoStream_line);
         if (_logFilePath && !verboseLog) 
         {
            *file << bufferInfoStream_line << endl;
         }
    }
    
    if (mainTreeE->icon(0).isNull())
    {
        mainTreeE->setTextAlignment(1, Qt::AlignVCenter | Qt::AlignRight);
        mainTreeE->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));
        mainTreeE->setText(1, "Undiscovered");
    }

    QTextStream bufferTraceStream(&bufferTraceroot); // Traceroute buffer
    QString bufferTraceStream_line;

    // check for traceroute scan information
    while (!bufferTraceStream.atEnd()) 
    {
        bufferTraceStream_line = bufferTraceStream.readLine();
        if (!bufferTraceStream_line.isEmpty() && !bufferTraceStream_line.contains("guessing hop")) 
        {
            elemObj->setTraceRouteInfo(bufferTraceStream_line);
            if (_logFilePath && !verboseLog) 
            {
                *file << bufferTraceStream_line << endl;
            }
        }
    }

    QTextStream bufferNssStream(&bufferNSS); // NSS
    QString bufferNssStream_line;

    // check for NSS scan information
    while (!bufferNssStream.atEnd()) 
    {
        bufferNssStream_line = bufferNssStream.readLine();
        if (!bufferNssStream_line.isEmpty()) 
        {
            elemObj->setNssInfo(bufferNssStream_line);
            if (_logFilePath && !verboseLog) 
            {
                *file << bufferNssStream_line << endl;
            }
        }
    }

    actionClear_History->setEnabled(true);

    QTextStream bufferLogStream(&StdoutStr);
    QString bufferLogStream_line;

    // check for full log scan
    while (!bufferLogStream.atEnd()) 
    {
        bufferLogStream_line = bufferLogStream.readLine();
        if (!bufferLogStream_line.isEmpty())
        {
            elemObj->setFullScanLog(bufferLogStream_line);
            if ((_logFilePath) && (verboseLog))
            {
                *file << bufferLogStream_line << "\n";
            }
        }
    }

    *file << "==LogEnd\n";
    if (_logFilePath) 
    {
        delete file;
    }

    QTextStream bufferErrorStream(&StderrorStr);
    QString bufferErrorStream_line;

    // check for scan error
    while (!bufferErrorStream.atEnd()) 
    {
        bufferErrorStream_line = bufferErrorStream.readLine();
        elemObj->setErrorScan(bufferErrorStream_line);
    }

    return elemObj;
}
