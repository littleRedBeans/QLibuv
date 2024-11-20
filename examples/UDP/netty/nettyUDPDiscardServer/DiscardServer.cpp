#include "DiscardServer.h"
#include <QDateTime>
#include <QDebug>
#include "QLibuv/UvUdpSocket.h"
using namespace shuimo::net;
using namespace std;
using namespace std::placeholders;
DiscardServer::DiscardServer(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(
          make_unique<UvUdpSocket>(addr_, port_, bind(&DiscardServer::onRecvData, this, _1, _2, _3)))
    , transferred_(0)
    , receivedMessages_(0)
    , oldCounter_(0)
    , startTime_(0)
{}

DiscardServer::~DiscardServer() {}
void DiscardServer::startRecv()
{
    //    connect(socket_.get(), &UvUdpSocket::sigRecv, this, &DiscardServer::onRecvData);
    connect(socket_.get(),
            &UvUdpSocket::sigError,
            this,
            [](const QString addr, quint16 port, const QString err) {
                qDebug() << tr("bind on %1:%2 error:%3").arg(addr).arg(port).arg(err);
            });
    socket_->initsocket();
    socket_->startRecv();
    startTime_ = QDateTime::currentMSecsSinceEpoch();
    connect(&timer_, &QTimer::timeout, this, &DiscardServer::printThroughput);
    timer_.start(3000);
}
void DiscardServer::printThroughput()
{
    qint64 endTime = QDateTime::currentMSecsSinceEpoch();
    qint64 newCounter = transferred_.load();
    qint64 msg = receivedMessages_.exchange(0);
    qint64 bytes = newCounter - oldCounter_;
    double time = (endTime - startTime_) / 1000.0;
    qDebug("%4.3f MiB/s %4.3f Ki Msg/s %6.2f bytes per msg,total recv %4.3f MiB\n",
           static_cast<double>(bytes) / time / 1024 / 1024,
           static_cast<double>(msg) / time / 1024,
           static_cast<double>(bytes) / static_cast<double>(msg),
           static_cast<double>(newCounter) / 1024 / 1024);
    oldCounter_ = newCounter;
    startTime_ = endTime;
}

void DiscardServer::onRecvData(const QString, quint16, QByteArray data)
{
    //    qDebug() << tr("DiscardServer recv %1:%2->%3:%4 %5bytes")
    //                    .arg(addr)
    //                    .arg(port)
    //                    .arg(addr_)
    //                    .arg(port_)
    //                    .arg(data.size());

    ++receivedMessages_;
    transferred_ += data.size();
}
