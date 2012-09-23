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

#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>

#include <config.h>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
             QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QString translationsPath;

#ifndef Q_WS_WIN
    translationsPath = TRANSLATIONS_PATH;
#else
    translationsPath.append(QDir::rootPath() + QDir::toNativeSeparators("program files/nmapsi4/"));
#endif

    QTranslator translator;
    bool translatorResult = translator.load("nmapsi4_" + QLocale::system().name(), translationsPath);

    if (translatorResult == false) {
        translatorResult = translator.load("nmapsi4_" + QLocale::system().name(), QDir::currentPath());
    }

    app.installTranslator(&translator);

    MainWindow* dialog = new MainWindow();
    dialog->show();
    return app.exec();
}
