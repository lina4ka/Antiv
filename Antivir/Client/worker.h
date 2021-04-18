#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <IO_.h>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QObject *_parent =0): QObject(_parent){};
    unique_ptr<Sender> innerSender;
    HANDLE iHandle;
    void _reconnect();
    bool isWork = true;
signals:
    void reconnect();
    void toDo(msg Message);
public slots:
    void doWork();
};

#endif // WORKER_H
