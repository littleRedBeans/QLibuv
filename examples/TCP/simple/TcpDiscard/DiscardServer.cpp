#include "DiscardServer.h"
#include <QDebug>
#include "QLibuv/UvTcpServer.h"
using namespace shuimo::net;
using namespace std;
using namespace std::placeholders;
DiscardServer::DiscardServer(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(make_unique<UvTcpServer>(addr_, port_))
{
    socket_->setRecvCallback(bind(&DiscardServer::onRecvData, this, _1, _2, _3));
}

DiscardServer::~DiscardServer() {}
void DiscardServer::startRecv()
{
    connect(socket_.get(),
            &UvTcpServer::sigError,
            this,
            [](const QString addr, quint16 port, const QString err) {
                qDebug() << tr("bind on %1:%2 error:%3").arg(addr).arg(port).arg(err);
            });
    socket_->startListen();
}

void DiscardServer::onRecvData(const QString addr, quint16 port, QByteArray data)
{
    qDebug() << tr("DiscardServer recv %1:%2->%3:%4 %5bytes")
                    .arg(addr)
                    .arg(port)
                    .arg(addr_)
                    .arg(port_)
                    .arg(data.size());
}
