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

#include "details.h"

classDetails::classDetails(QStringList& processFlow, const QString hostname): scanLines(processFlow)
{
    setupUi(this);
    monitorEditHostname->setText(hostname);
    m_itemsSize = processFlow.size();
    m_timer = new QTimer(this);
    connect(monitorCloseButt, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(monitorReloadButt, SIGNAL(clicked(bool)), this, SLOT(reloadFlow()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(reloadFlow()));
    loadFlow();
}

classDetails::~classDetails()
{
    freelist<QListWidgetItem*>::itemDeleteAll(itemsList);
    m_timer->stop();
    delete m_timer;
}

void classDetails::loadFlow()
{
    foreach (const QString &token, scanLines) 
    {
        QListWidgetItem *item_ = new QListWidgetItem(detailsListW);
        itemsList.push_back(item_);
        if (token.contains("open")) 
        {
            item_->setForeground(QBrush(QColor(0, 0, 255, 127)));
        } 
        else if (token.contains("closed")) 
        {
            item_->setForeground(QBrush(QColor(255, 0, 0, 127)));
        } 
        else if (token.contains("filtered") || token.contains("unfiltered")) 
        {
            item_->setForeground(QBrush(QColor(255, 134, 12, 127)));
        }

        item_->setText(token);
    }
    
    // Start QTimer for automatic reload
    m_timer->start(4000);
}

void classDetails::reloadFlow()
{
    if (m_itemsSize < scanLines.size()) 
    {
        for (int i = m_itemsSize; i < scanLines.size(); ++i) 
        {
            QListWidgetItem *item_ = new QListWidgetItem(detailsListW);
            itemsList.push_back(item_);
            if (scanLines[i].contains("open")) 
            {
                item_->setForeground(QBrush(QColor(0, 0, 255, 127)));
            } 
            else if (scanLines[i].contains("closed")) 
            {
                item_->setForeground(QBrush(QColor(255, 0, 0, 127)));
            } 
            else if (scanLines[i].contains("filtered") || scanLines[i].contains("unfiltered")) 
            {
                item_->setForeground(QBrush(QColor(255, 134, 12, 127)));
            }

            item_->setText(scanLines[i]);
        }
        m_itemsSize = scanLines.size();
    }
}
