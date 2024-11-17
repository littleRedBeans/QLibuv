#include "EchoClient.h"
#include <QDateTime>
#include <QDebug>
#include <unistd.h>
using namespace std;
EchoClient::EchoClient(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(make_unique<QUdpSocket>(this))
{}

EchoClient::~EchoClient() {}
void EchoClient::startRecv()
{
    connect(socket_.get(), &QUdpSocket::readyRead, this, &EchoClient::onRecvData);
    socket_->bind(QHostAddress(addr_), port_);
    QByteArray data(256, 'H');
    socket_->writeDatagram(data, QHostAddress(addr_), 2007);
}

void EchoClient::onRecvData()
{
    //    qDebug() << tr("EchoClient recv %1:%2->%3:%4 %5bytes")
    //                    .arg(addr)
    //                    .arg(port)
    //                    .arg(addr_)
    //                    .arg(port_)
    //                    .arg(data.size());
    //    qDebug() << "onRecvData tid:" << gettid();
    while (socket_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket_->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;
        qint64 recv = socket_->readDatagram(datagram.data(),
                                            datagram.size(),
                                            &senderAddress,
                                            &senderPort);
        socket_->writeDatagram(datagram, senderAddress, senderPort);
    }
}
