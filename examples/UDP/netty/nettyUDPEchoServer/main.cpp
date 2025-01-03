#include <QCoreApplication>
#include <QDebug>
#include "EchoServer.h"
#include <unistd.h>
int main(int argc, char *argv[])
{
    qDebug() << "main thread:" << getpid();
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString addr(argv[1]);
        EchoServer server(addr, 2007);
        server.startRecv();
        return a.exec();
    } else {
        printf("Usage: %s addr\n", argv[0]);
    }
    return -1;
}
