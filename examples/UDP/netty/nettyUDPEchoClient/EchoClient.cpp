#include "EchoClient.h"
#include <QDateTime>
#include <QDebug>
#include "QLibuv/UvUdpSocket.h"
using namespace shuimo::net;
using namespace std;
using namespace std::placeholders;
EchoClient::EchoClient(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(
          make_unique<UvUdpSocket>(addr_, port_, bind(&EchoClient::onRecvData, this, _1, _2, _3)))
{}

EchoClient::~EchoClient() {}
void EchoClient::startRecv()
{
    //    connect(socket_.get(), &UvUdpSocket::sigRecv, this, &EchoClient::onRecvData);
    connect(socket_.get(),
            &UvUdpSocket::sigError,
            this,
            [](const QString addr, quint16 port, const QString err) {
                qDebug() << tr("bind on %1:%2 error:%3").arg(addr).arg(port).arg(err);
            });
    socket_->initsocket();
    socket_->startRecv();
}

void EchoClient::sendData()
{
    QByteArray data(256, 'H');
    socket_->sendData(data, addr_, 2007);
}

void EchoClient::onRecvData(const QString addr, quint16 port, QByteArray data)
{
    //    qDebug() << tr("EchoClient recv %1:%2->%3:%4 %5bytes")
    //                    .arg(addr)
    //                    .arg(port)
    //                    .arg(addr_)
    //                    .arg(port_)
    //                    .arg(data.size());
    socket_->sendData(data, addr, port);
}
