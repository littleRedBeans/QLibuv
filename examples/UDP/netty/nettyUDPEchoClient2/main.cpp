#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include "EchoClient.h"
#include <unistd.h>
int main(int argc, char *argv[])
{
    qDebug() << "main tid:" << gettid();
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString addr(argv[1]);
        EchoClient server(addr, 3007);
        QThread *thread = new QThread;
        thread->setObjectName("networkThread");
        server.moveToThread(thread);
        thread->start();
        QTimer::singleShot(0, &server, &EchoClient::startRecv);
        return a.exec();
    } else {
        printf("Usage: %s addr\n", argv[0]);
    }
    return -1;
}
