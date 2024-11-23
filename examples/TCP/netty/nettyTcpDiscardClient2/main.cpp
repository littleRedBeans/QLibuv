#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include "DiscardClient.h"
#include <functional>
#include <unistd.h>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
    qDebug() << "main tid:" << gettid();
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        QString addr(argv[1]);
        vector<QThread *> threadPool;
        for (int i = 0; i < 8; ++i) {
            threadPool.push_back(new QThread);
            threadPool[i]->setObjectName(QString("networkThread%1").arg(i));
            threadPool[i]->start();
        }
        for (int i = 0; i < 512; ++i) {
            DiscardClient *client = new DiscardClient(addr, 2009);
            client->moveToThread(threadPool[i % 8]);
            QTimer::singleShot(0, client, &DiscardClient::startRecv);
        }
        return a.exec();
    } else {
        printf("Usage: %s addr\n", argv[0]);
    }
    return -1;
}
