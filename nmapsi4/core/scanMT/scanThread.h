#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QThread>
#include <QByteArray>
#include <QStringList>
#include <QProcess>
#include <QObject>
//#include <QtGui>
#include <QMetaType>
#include <QtDebug>
#include <QMutex>

class scanThread : public QThread
{
 Q_OBJECT

 public:
     scanThread(QByteArray* ProcB1, QByteArray* ProcB2, 
		QStringList parametri, QObject *parent = 0);

signals:
     void threadEnd(QString);
     void upgradePR();

private:
     QByteArray* pout;
     QByteArray* perr;
     QStringList ParList;
     QMutex mutexLocal;

private slots:
     void setValue();
     void stopProcess();

 protected:
     QProcess *proc;
     void run();
     QObject* par;
};


#endif
