#include "MultiCastServer.h"
#include <QDateTime>
#include <QDebug>
#include "QLibuv/UvUdpSocket.h"
using namespace shuimo::net;
using namespace std;
MultiCastServer::MultiCastServer(const QString &bindAddr,
                                 const QString &interfaceAddr,
                                 quint16 port,
                                 QObject *parent)
    : QObject{parent}
    , addr_(bindAddr)
    , interfaceAddr_(interfaceAddr)
    , port_(port)
    , socket_(make_unique<UvUdpSocket>(addr_, port_))
{}

MultiCastServer::~MultiCastServer() {}
void MultiCastServer::startRecv(QStringList multicastAddrs)
{
    connect(socket_.get(), &UvUdpSocket::sigRecv, this, &MultiCastServer::onRecvData);
    connect(socket_.get(),
            &UvUdpSocket::sigError,
            this,
            [](const QString addr, quint16 port, const QString err) {
                qDebug() << tr("bind on %1:%2 error:%3").arg(addr).arg(port).arg(err);
            });
    socket_->initsocket();
    for (const auto &addr : multicastAddrs) {
        socket_->joinMultiCast(addr, interfaceAddr_);
    }
    socket_->startRecv();
}

void MultiCastServer::onRecvData(const QString addr, quint16 port, QByteArray data)
{
    qDebug() << tr("MultiCastServer recv %1:%2->%3:%4 %5bytes")
                    .arg(addr)
                    .arg(port)
                    .arg(addr_)
                    .arg(port_)
                    .arg(data.size());
    socket_->sendData(data, addr, port);
}
