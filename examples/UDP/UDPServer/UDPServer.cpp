#include "UDPServer.h"
#include "QLibuv/UvUdpSocket.h"
using namespace shuimo::net;
using namespace std;
UDPServer::UDPServer(QString addr, quint16 port, QObject *parent)
    : QObject(parent)
    , addr_(addr)
    , port_(port)
    , socket_(make_unique<UvUdpSocket>(addr_, port_))
{
}
UDPServer::~UDPServer() {}

void UDPServer::startRecv()
{
    socket_->startRecv();
}
