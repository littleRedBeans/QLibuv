#include "DiscardServer.h"
#include <QDateTime>
#include <QDebug>
#include <unistd.h>
using namespace std;
DiscardServer::DiscardServer(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(make_unique<QUdpSocket>(this))
    , transferred_(0)
    , receivedMessages_(0)
    , oldCounter_(0)
    , startTime_(0)
    , timer_(this)
{}

DiscardServer::~DiscardServer() {}
void DiscardServer::startRecv()
{
    connect(socket_.get(), &QUdpSocket::readyRead, this, &DiscardServer::onRecvData);
    socket_->bind(QHostAddress(addr_), port_);
    startTime_ = QDateTime::currentMSecsSinceEpoch();
    connect(&timer_, &QTimer::timeout, this, &DiscardServer::printThroughput);
    timer_.start(3000);
}
void DiscardServer::printThroughput()
{
    qint64 endTime = QDateTime::currentMSecsSinceEpoch();
    qint64 bytes = transferred_ - oldCounter_;
    double time = (endTime - startTime_) / 1000.0;
    qDebug("%4.3f MiB/s %4.3f Ki Msg/s %6.2f bytes per msg,total recv %4.3f MiB\n",
           static_cast<double>(bytes) / time / 1024 / 1024,
           static_cast<double>(receivedMessages_) / time / 1024,
           static_cast<double>(bytes) / static_cast<double>(receivedMessages_),
           static_cast<double>(transferred_) / 1024 / 1024);
    receivedMessages_ = 0;
    oldCounter_ = transferred_;
    startTime_ = endTime;
}

void DiscardServer::onRecvData()
{
    //    qDebug() << tr("DiscardServer recv %1:%2->%3:%4 %5bytes")
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
        ++receivedMessages_;
        transferred_ += recv;
    }
}
