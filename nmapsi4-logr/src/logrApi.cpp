#include "mainwin.h"


QString mwClass::showBrowser()
{

    QString url = QFileDialog::getOpenFileName(this, tr("Open Log"),
                  QDir::homePath(), tr("Log (*.log)"));

    return url;
}

void mwClass::itemDeleteAll(QList<QTreeWidgetItem*> items)
{
    qDebug() << "nwClass/itemDeleteAll() -> Free List";
    qDeleteAll(items);
    items.clear();
}

void mwClass::exit()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("window/pos", pos());
    settings.setValue("window/size", size());
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
