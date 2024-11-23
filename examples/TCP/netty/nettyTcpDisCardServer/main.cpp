#include <QCoreApplication>
#include "DiscardServer.h"
#include <unistd.h>
int main(int argc, char *argv[])
{
    qDebug() << "main tid:" << getpid();
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString addr(argv[1]);
        DiscardServer server(addr, 2009);
        server.startRecv();
        return a.exec();
    } else {
        printf("Usage: %s addr\n", argv[0]);
    }
    return -1;
}