#include "mainwin.h"


mwClass::mwClass() {
     setupUi(this);

     connect( actionOpen, SIGNAL(triggered()), 
	      this, SLOT(logReader()));

     // Take a nmapsi4 geometry info
     QSettings settings("nmapsi4","nmapsi4");
     QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
     QSize size = settings.value("window/size", QSize(869, 605)).toSize();
     resize(size);
     move(pos);
}

void mwClass::logReader() {
     // TODO
     // Read log
     if((url = showBrowser()).isEmpty())
	  return;

     QFile *logF = new QFile();
     qDebug() << "nmapsi4-logr:: --> url::" << url;
     logF->setFileName(url);
     if(!logF->open(QIODevice::ReadOnly))
	  qDebug() << "Log File open error." << endl;

     QTextStream buffer(logF);
     QString tmpLine;

     treeLogView->setIconSize(QSize::QSize (48, 48));

     while( !buffer.atEnd() ) {
	  tmpLine = buffer.readLine();

	  if(tmpLine.contains("Log Start")) {
	       root = new QTreeWidgetItem(treeLogView);
	       ItemList.push_front(root);
	       root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
	       tmpLine = buffer.readLine();
	       root->setText(0,tmpLine);
	       while( !tmpLine.contains("Log End") ) {
		    item = new QTreeWidgetItem(root);
		    ItemList.push_front(item);
		    tmpLine = buffer.readLine();
		    
                    // FIXME Scan info
		    if(!tmpLine.contains("Log End"))
			 item->setText(0,tmpLine);

	       }
	  }
     }

     logF->close();
     delete logF;

}

mwClass::~mwClass() 
{
     itemDeleteAll(ItemList);
}
