#include <QCoreApplication>
#include <QDebug>
#include "MultiCastServer.h"
#include <unistd.h>
int main(int argc, char *argv[])
{
    qDebug() << "MultiCastServer pid:" << getpid();
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString interfaceAddr(argv[1]);
        //bind on 0.0.0.0 for listening all addr
        MultiCastServer server("0.0.0.0", interfaceAddr, 2007);
        //multicast addr 224.0.0.0～239.255.255.255
        QStringList multicastAddrs = {"224.0.0.1", "224.0.0.2", "224.0.0.3"};
        server.startRecv(multicastAddrs);
        return a.exec();
    } else {
        printf("Usage: %s interfaceAddr\n", argv[0]);
    }
    return -1;
}
