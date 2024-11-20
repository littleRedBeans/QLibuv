#include <QCoreApplication>
#include <QTimer>
#include "EchoClient.h"
int main(int argc, char *argv[])
{
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString addr(argv[1]);
        for (int i = 0; i < 10; ++i) {
            EchoClient *client = new EchoClient(addr, 3000 + i);
            client->startRecv();
            QTimer::singleShot(2000, client, &EchoClient::sendData);
        }
        return a.exec();
    } else {
        printf("Usage: %s addr\n", argv[0]);
    }
    return -1;
}
