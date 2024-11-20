#include "DiscardClient.h"
#include <QDateTime>
#include <QDebug>
#include <unistd.h>
using namespace std;
DiscardClient::DiscardClient(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(make_unique<QUdpSocket>(this))
{}

DiscardClient::~DiscardClient() {}
void DiscardClient::startRecv()
{
    socket_->bind(QHostAddress(addr_), port_);
    QByteArray data(1024, 'H');
    for (int i = 0; i < 10000 * 1024; ++i) {
        socket_->writeDatagram(data, QHostAddress(addr_), 2007);
    }
    qDebug() << "send finished...";
}
