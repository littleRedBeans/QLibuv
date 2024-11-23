#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include "DiscardServer.h"
#include <unistd.h>
int main(int argc, char *argv[])
{
    qDebug() << "main tid:" << gettid();
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString addr(argv[1]);
        DiscardServer server(addr, 2009);
        QThread *thread = new QThread;
        thread->setObjectName("networkThread");
        server.moveToThread(thread);
        thread->start();
        QTimer::singleShot(0, &server, &DiscardServer::startRecv);
        return a.exec();
    } else {
        printf("Usage: %s addr\n", argv[0]);
    }
    return -1;
}
