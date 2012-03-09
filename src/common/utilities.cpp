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

#include "utilities.h"
#include "mainwindow.h"

Utilities::Utilities(MainWindow* parent)
: QObject(parent), m_ui(parent)
{
    connect(m_ui->action_About, SIGNAL(triggered()),
            this, SLOT(about()));    // about action menu
    connect(m_ui->actionAbout_Qt, SIGNAL(triggered()),
            this, SLOT(aboutQt()));    // about action menu

        // action help menu (browser call)
    connect(m_ui->actionReport_Bug, SIGNAL(triggered()),
            this, SLOT(showBugUrl()));
    connect(m_ui->actionVisit_Website, SIGNAL(triggered()),
            this, SLOT(showHomepageUrl()));
    connect(m_ui->actionDocumentation, SIGNAL(triggered()),
            this, SLOT(showDocumentationUrl()));
    connect(m_ui->actionDonate_Money, SIGNAL(triggered()),
            this, SLOT(showDonateUrl()));
}

Utilities::~Utilities()
{
}

void Utilities::openFileBrowser(QLineEdit *destination)
{
    showBrowser(destination);
}

void Utilities::showBrowser(QLineEdit *destination) // private
{
    QString FileName = QFileDialog::getOpenFileName(m_ui, tr("Select the file"), "/home", "");

    destination->setText(FileName);
}

void Utilities::openDirectoryDialog(QLineEdit *destination)
{
    QString url = QDir::homePath();

    QString FileName = QFileDialog::getExistingDirectory(m_ui, "Open Directory",
               url, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(FileName.isEmpty())
    {
        FileName.append(QDir::tempPath());
    }

    destination->setText(FileName);
}

void Utilities::about()
{
    QWeakPointer<About> about = new About(m_ui);
    about.data()->exec();

    if (!about.isNull())
    {
        delete about.data();
    }
}

void Utilities::aboutQt()
{
    QMessageBox::aboutQt(m_ui, "Qt Version");
}

void Utilities::showBugUrl()
{
    QDesktopServices::openUrl(QUrl("https://launchpad.net/nmapsi4"));
}

void Utilities::showHomepageUrl()
{
    QDesktopServices::openUrl(QUrl("http://www.nmapsi4.org"));
}

void Utilities::showDocumentationUrl()
{
    QDesktopServices::openUrl(QUrl(""));
}

void Utilities::showDonateUrl()
{
    QDesktopServices::openUrl(QUrl("http://www.nmapsi4.org"));
}
