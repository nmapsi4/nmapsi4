/*
Copyright 2011-2015  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "monitorhostscandetails.h"

MonitorDetails::MonitorDetails(QStringList& processFlow, const QString hostname, QWidget* parent)
    : QDialog(parent), m_scanLines(processFlow)
{
    setupUi(this);
    monitorEditHostname->setText(hostname);
    m_itemsSize = processFlow.size();
    m_timer = new QTimer(this);
    connect(monitorCloseButt, &QPushButton::clicked, this, &MonitorDetails::close);
    connect(monitorReloadButt, &QPushButton::clicked, this, &MonitorDetails::reloadFlow);
    connect(m_timer, &QTimer::timeout, this, &MonitorDetails::reloadFlow);
    loadFlow();
}

MonitorDetails::~MonitorDetails()
{
    memory::freelist<QListWidgetItem*>::itemDeleteAll(m_itemsList);
    m_timer->stop();
    delete m_timer;
}

void MonitorDetails::loadFlow()
{
    for (const QString & token : m_scanLines) {
        QListWidgetItem *item_ = new QListWidgetItem(detailsListW);
        m_itemsList.push_back(item_);
        if (token.contains("open")) {
            item_->setForeground(QBrush(QColor(0, 0, 255, 127)));
        } else if (token.contains("closed")) {
            item_->setForeground(QBrush(QColor(255, 0, 0, 127)));
        } else if (token.contains("filtered") || token.contains("unfiltered")) {
            item_->setForeground(QBrush(QColor(255, 134, 12, 127)));
        }

        item_->setText(token);
    }

    // Start QTimer for automatic reload
    m_timer->start(4000);
}

void MonitorDetails::reloadFlow()
{
    if (m_itemsSize < m_scanLines.size()) {
        for (int i = m_itemsSize; i < m_scanLines.size(); ++i) {
            QListWidgetItem *item_ = new QListWidgetItem(detailsListW);
            m_itemsList.push_back(item_);
            if (m_scanLines[i].contains("open")) {
                item_->setForeground(QBrush(QColor(0, 0, 255, 127)));
            } else if (m_scanLines[i].contains("closed")) {
                item_->setForeground(QBrush(QColor(255, 0, 0, 127)));
            } else if (m_scanLines[i].contains("filtered") || m_scanLines[i].contains("unfiltered")) {
                item_->setForeground(QBrush(QColor(255, 134, 12, 127)));
            }

            item_->setText(m_scanLines[i]);
        }
        m_itemsSize = m_scanLines.size();
    }
}
