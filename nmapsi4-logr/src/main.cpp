#include <QApplication>
#include "mainwin.h"


int main(int argc, char *argv[])
{
//    Q_INIT_RESOURCE(mwClass);
    QApplication app(argc, argv);

/*    QString locale = QLocale::system().name();
    QString localeComplete;
    localeComplete.append("nmapsi4_");
    locale.resize(2);
    localeComplete.append(locale);

    QTranslator translator;
    bool tmp_translator;
    QString urlTranslate;

#ifndef Q_WS_WIN // Gnu/Linux && Mac Os/x
    urlTranslate = __LinuxTranslateUrl__;
    qDebug() << "Nmapsi4/core -> url Translator::" << urlTranslate;

#else // MS Windows
    urlTranslate = QDir::homePath(); // Insert a url for MS Windows Translate ( FIXME check for local binary path)
    qDebug() << "Nmapsi4/core -> url Translator::" << urlTranslate;

#endif

    tmp_translator = translator.load(localeComplete, urlTranslate);	
    
    // debug messages for Translation file
    qDebug() << "Nmapsi4/core -> Locale::" << localeComplete;
    qDebug() << "Nmapsi4/core -> Translation file Load::" << tmp_translator;
    qDebug() << "Nmapsi4/core -> Traslation file is Empty::" << translator.isEmpty();

    app.installTranslator(&translator);*/

    mwClass dialog;
    dialog.show();
    return app.exec();
}
