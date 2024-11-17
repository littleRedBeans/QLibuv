#include "DaytimeServer.h"
#include <QDateTime>
#include <QDebug>
#include "QLibuv/UvUdpSocket.h"
using namespace shuimo::net;
using namespace std;
DaytimeServer::DaytimeServer(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(make_unique<UvUdpSocket>(addr_, port_))
{}

DaytimeServer::~DaytimeServer() {}
void DaytimeServer::startRecv()
{
    connect(socket_.get(), &UvUdpSocket::sigRecv, this, &DaytimeServer::onRecvData);
    connect(socket_.get(),
            &UvUdpSocket::sigError,
            this,
            [](const QString addr, quint16 port, const QString err) {
                qDebug() << tr("bind on %1:%2 error:%3").arg(addr).arg(port).arg(err);
            });
    socket_->initsocket();
    socket_->startRecv();
}

void DaytimeServer::onRecvData(const QString addr, quint16 port, const QByteArray data)
{
    qDebug() << tr("DaytimeServer recv %1:%2->%3:%4 %5bytes")
                    .arg(addr)
                    .arg(port)
                    .arg(addr_)
                    .arg(port_)
                    .arg(data.size());
    QByteArray msg = QDateTime::currentDateTime().toString().toLocal8Bit();
    msg.append('\n');
    socket_->sendData(msg, addr, port);
}
