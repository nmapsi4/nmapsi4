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

#include "mainwin.h"


void nmapClass::showParserObjPlugins(int hostIndex) 
{
    // show traceroute
    foreach (const QString &token, parserObjList[hostIndex]->getTraceRouteInfo()) 
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeTraceroot);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));
        QStringList tmpToken = token.split(' ');

        tmpToken.removeAll("");

        // MS windows check for ms string
        if(tmpToken.size() == 5) 
        {
            if(tmpToken[2].size() < 4) 
            { // minimum dns length
                tmpToken.removeAt(2);
            }
        }

        if(tmpToken.size() == 4) 
        {
            if(tmpToken[2].size() < 4) 
            { // minimum dns length
                tmpToken.removeAt(2);
            } 
            else 
            {
                tmpToken[3].remove('(');
                tmpToken[3].remove(')');
            }
        }

        if(tmpToken.size() == 4) 
        {
            root->setText(0, tmpToken[0]);
            root->setText(1, tmpToken[1]);
            root->setText(3, tmpToken[2]);
            root->setText(2, tmpToken[3]);

        } 
        else if(tmpToken.size() == 3) 
        {
            root->setText(0, tmpToken[0]);
            root->setText(1, tmpToken[1]);
            root->setText(2, tmpToken[2]);
            root->setText(3, "no DNS");
            root->setForeground(3, QBrush(QColor(255, 0, 0, 127)));
        } 
        else 
        {
            root->setText(0, token);
            root->setToolTip(0, token);
        }
    }
    
    // show lookUp info
    foreach (parserObjUtil* elem, parserObjUtilList) 
    {
        if(parserObjList[hostIndex]->getHostName() == elem->getHostName()) 
        {
            foreach (const QString &token, elem->getInfoLookup()) 
            {
                QTreeWidgetItem *root = new QTreeWidgetItem(treeLookup);
                itemListScan.push_front(root);
                root->setSizeHint(0, QSize(22, 22));
                root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
                root->setText(0, token);
            }
            break;
        }
    }
}
