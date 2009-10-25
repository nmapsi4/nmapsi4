#include "mainwin.h"


QString mwClass::showBrowser()
{

    QString url = QFileDialog::getOpenFileName(this, tr("Open Log"),
                  QDir::homePath(), tr("Log (*.log)"));

    return url;
}

void mwClass::itemDeleteAll(QList<QTreeWidgetItem*> items)
{
#ifndef LOGR_NO_DEBUG
    qDebug() << "nwClass/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void mwClass::exit()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("window-logr/pos", pos());
    settings.setValue("window-logr/size", size());
    close();
}


void mwClass::logFromHistory()
{
    if (toolBoxMain->currentIndex()) {
        url = logTree->currentItem()->text(1);
        url.append(logTree->currentItem()->text(0));
        logReader();
        toolBoxMain->setCurrentIndex(0);
    }
}

void mwClass::about()
{
    mainAbout about;
    about.exec();
}

void mwClass::about_qt()
{
    mainAbout about;
    about.qt();
}
