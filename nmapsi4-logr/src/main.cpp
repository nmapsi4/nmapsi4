/***************************************************************************
 *   Copyright (C) 2008 by Francesco Cecconi                               *
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

#include <QApplication>
#include <QTranslator>
#include "mainwin.h"
#include "config.h"


int main(int argc, char *argv[])
{
//    Q_INIT_RESOURCE(mwClass);
    QApplication app(argc, argv);

    QString locale = QLocale::system().name();
    QString localeComplete;
    localeComplete.append("nmapsi4-logr_");
    locale.resize(2);
    localeComplete.append(locale);

    QTranslator translator;
    bool tmp_translator;
    QString urlTranslate;

#ifndef Q_WS_WIN // Gnu/Linux && Mac Os/x
    urlTranslate = __LinuxTranslateUrl__;
    qDebug() << "Nmapsi4-logr/core -> url Translator::" << urlTranslate;

#else // MS Windows
    urlTranslate = QDir::homePath(); // Insert a url for MS Windows Translate ( FIXME check for local binary path)
    qDebug() << "Nmapsi4-logr/core -> url Translator::" << urlTranslate;

#endif

    tmp_translator = translator.load(localeComplete, urlTranslate);

    if(tmp_translator == false)
	 tmp_translator = translator.load(localeComplete, QDir::currentPath());	
    
    // debug messages for Translation file
    qDebug() << "Nmapsi4-logr/core -> Locale::" << localeComplete;
    qDebug() << "Nmapsi4-logr/core -> Translation file Load::" << tmp_translator;
    qDebug() << "Nmapsi4-logr/core -> Traslation file is Empty::" << translator.isEmpty();

    app.installTranslator(&translator);

    mwClass dialog;
    dialog.show();
    return app.exec();
}
