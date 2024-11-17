#include <QCoreApplication>
#include "../UDPDaytime/DaytimeServer.h"
#include "../UDPDiscard/DiscardServer.h"
#include "../UDPEcho/EchoServer.h"
#include "../UDPtime/TimeServer.h"
int main(int argc, char *argv[])
{
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString addr(argv[1]);

        TimeServer timeServer(addr, 2037);
        timeServer.startRecv();

        DaytimeServer daytimeServer(addr, 2013);
        daytimeServer.startRecv();

        EchoServer echoServer(addr, 2007);
        echoServer.startRecv();

        DiscardServer discardServer(addr, 2009);
        discardServer.startRecv();
        return a.exec();
    } else {
        printf("Usage: %s addr\n", argv[0]);
    }
    return -1;
}
