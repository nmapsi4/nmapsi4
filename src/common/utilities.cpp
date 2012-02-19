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

Utilities::Utilities(QWidget* parent)
: QWidget(parent), m_parent(parent)
{
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
    QString FileName = QFileDialog::getOpenFileName(m_parent, tr("Select the file"), "/home", "");

    destination->setText(FileName);
}

void Utilities::openDirectoryDialog(QLineEdit *destination)
{
    QString url = QDir::homePath();

    QString FileName = QFileDialog::getExistingDirectory(m_parent, "Open Directory",
               url, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(FileName.isEmpty())
    {
        FileName.append(QDir::tempPath());
    }

    destination->setText(FileName);
}

void Utilities::about()
{
    QWeakPointer<About> about = new About(this);
    about.data()->exec();

    if (!about.isNull())
    {
        delete about.data();
    }
}

void Utilities::aboutQt()
{
    QMessageBox::aboutQt(this, "Qt Version");
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
