#include <QCoreApplication>
#include "DaytimeServer.h"
int main(int argc, char *argv[])
{
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString addr(argv[1]);
        DaytimeServer server(addr, 2013);
        server.startRecv();
        return a.exec();
    } else {
        printf("Usage: %s addr\n", argv[0]);
    }
    return -1;
}
