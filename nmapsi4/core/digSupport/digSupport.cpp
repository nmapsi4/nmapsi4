/***************************************************************************
 *   Copyright (C) 2009-2011 by Francesco Cecconi                          *
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

#include "digSupport.h"

digSupport::digSupport() {
    state = false;
}


void digSupport::checkDigSupport() {
    digProc = new QProcess();

    QStringList parametri;
    parametri << "-v";
    digProc->start("dig", parametri);

    connect(digProc, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(checkDig()));
}

void digSupport::checkDig() {

    QString *output, *error;
    output = new QString(digProc->readAllStandardOutput());
    error = new QString(digProc->readAllStandardError());

    QTextStream stream(output);
    QTextStream stream2(error);
    QString line, line2;

    line = stream.readLine();
    line2 = stream2.readLine();
#ifndef DIG_NO_DEBUG
    qDebug() << "Dig::line:: " << line;
    qDebug() << "Dig::error:: " << line2;
#endif
    if (line2.startsWith("DiG") || line.startsWith("DiG")) {
        state = true;
#ifndef DIG_NO_DEBUG
        qDebug() << "Dig support enable";
#endif
    }

    delete output;
    delete error;
    delete digProc;
}

bool digSupport::getDigSupport() {
    return state;
}

void digSupport::digProcess(const QString hostname, QTreeWidget* view) {
    Wview = view;
    QByteArray buff1;
    QStringList command;
    command << hostname;
    th = new digThread(buff1, command, this);
    th->start();
    connect(th, SIGNAL(threadEnd(const QStringList, QByteArray)),
      this, SLOT(digReturn(const QStringList, QByteArray)));
}

void digSupport::digReturn(const QStringList hostname, QByteArray buffer1) {
#ifndef DIG_NO_DEBUG
    qDebug() << "############## digSupport():: start pars ######################";
#endif

    QString buff1(buffer1);

    QTextStream stream1(&buff1);
    QString line;

    Wview->setIconSize(QSize(32, 32));
    QTreeWidgetItem *rootLook = new QTreeWidgetItem(Wview);
    digList.push_front(rootLook);


    QTreeWidgetItem *itemLook;

    rootLook->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_result.png")));
    rootLook->setText(0, hostname[0]);

    bool resultFlag = false;
    while(!stream1.atEnd()) {
        line = stream1.readLine();
        if(!line.startsWith(";;") && !line.startsWith(";") && !line.isEmpty()) {
#ifndef DIG_NO_DEBUG
            qDebug() << "digSupport():: " << line;
#endif
            itemLook = new QTreeWidgetItem(rootLook);
            digList.push_front(itemLook);
            itemLook->setSizeHint(0, QSize(22, 22));
            itemLook->setIcon(0, QIcon(QString::fromUtf8(":/images/images/view-web-browser-dom-tree.png")));
            itemLook->setText(0,line);
            if(!resultFlag) {
                resultFlag = true;
            }

        }
    }

    if(!resultFlag) {
        rootLook->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    delete th;
    buffer1.clear();
}


digSupport::~digSupport() {
    qDeleteAll(digList);
    digList.clear();
}

