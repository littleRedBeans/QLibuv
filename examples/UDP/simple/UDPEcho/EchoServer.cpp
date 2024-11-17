#include "EchoServer.h"
#include <QDateTime>
#include <QDebug>
#include "QLibuv/UvUdpSocket.h"
using namespace shuimo::net;
using namespace std;
EchoServer::EchoServer(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(make_unique<UvUdpSocket>(addr_, port_))
{}

EchoServer::~EchoServer() {}
void EchoServer::startRecv()
{
    connect(socket_.get(), &UvUdpSocket::sigRecv, this, &EchoServer::onRecvData);
    connect(socket_.get(),
            &UvUdpSocket::sigError,
            this,
            [](const QString addr, quint16 port, const QString err) {
                qDebug() << tr("bind on %1:%2 error:%3").arg(addr).arg(port).arg(err);
            });
    socket_->initsocket();
    socket_->startRecv();
}

void EchoServer::onRecvData(const QString addr, quint16 port, QByteArray data)
{
    qDebug() << tr("EchoServer recv %1:%2->%3:%4 %5bytes")
                    .arg(addr)
                    .arg(port)
                    .arg(addr_)
                    .arg(port_)
                    .arg(data.size());
    socket_->sendData(data, addr, port);
}
