#include "UvTcpServer.h"
#include <QDebug>
#include "EventDispatcherLibUv.h"
#include "NetManager.h"
#include <unistd.h>
using namespace shuimo::net;
using namespace std;
using namespace std::placeholders;

UvTcpServer::UvTcpServer(const QString &ip, quint16 port, RecvCallback recvCb, QObject *parent)
    : QObject(parent)
    , addr_(ip)
    , port_(port)
    , recvCb_(recvCb)
    , start_(false)
{
    NetManager::instance()->moveToThread(this);
}

UvTcpServer::~UvTcpServer() {}

void UvTcpServer::sendData(QByteArray &data, const QString &address, quint16 port)
{
    QMetaObject::invokeMethod(this,
                              "invokeSendData",
                              Q_ARG(QByteArray, data),
                              Q_ARG(const QString &, address),
                              Q_ARG(quint16, port));
}

void UvTcpServer::invokeSendData(QByteArray data, const QString &address, quint16 port) {}

void UvTcpServer::startListen()
{
    QMetaObject::invokeMethod(this, "invokeStartListen");
}
void UvTcpServer::onNewConnection(uv_stream_t *server, int status)
{
    UvTcpServer *socket = static_cast<UvTcpServer *>(uv_handle_get_data((uv_handle_t *) server));
    if (status < 0) {
        socket->sigError(socket->addr_, socket->port_, uv_err_name(status));
        return;
    }

    uv_tcp_t *client = new uv_tcp_t;
    client->data = socket;
    uv_tcp_init(socket->loop_, client);
    if (uv_accept(server, (uv_stream_t *) client) == 0) {
        uv_read_start((uv_stream_t *) client, allocBuffer, onRead);
    } else {
        uv_close((uv_handle_t *) client, nullptr);
    }
}

void UvTcpServer::onRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
    UvTcpServer *socket = static_cast<UvTcpServer *>(client->data);
    if (nread > 0) {
        struct sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        uv_tcp_getpeername((uv_tcp_t *) client, (struct sockaddr *) &clientAddr, &clientLen);
        char clientIp[17] = {0};
        uv_ip4_name(&clientAddr, clientIp, 16);
        socket->recvCallback_(clientIp, ntohs(clientAddr.sin_port), QByteArray(buf->base, nread));
    } else if (nread < 0) {
        if (nread != UV_EOF) {
            socket->sigError(socket->addr_, socket->port_, uv_err_name(nread));
        }
        uv_close((uv_handle_t *) client, nullptr);
    }
}
void UvTcpServer::invokeStartListen()
{
    assert(false == start_);
    start_ = true;
    //create uv_loop_t uv_tcp_t

    EventDispatcherLibUv *dispatcher = static_cast<EventDispatcherLibUv *>(
        QAbstractEventDispatcher::instance());
    loop_ = dispatcher->uvLoop();
    tcp_socket_ = make_unique<uv_tcp_t>();

    int r = uv_tcp_init(loop_, tcp_socket_.get());
    if (r != 0) {
        emit sigError(addr_, port_, uv_err_name(r));
        return;
    }
    //set context UvTcpServer pointer to tcp_socket_
    uv_handle_set_data((uv_handle_t *) tcp_socket_.get(), this);

    struct sockaddr_in addr;
    std::string tmpAddr = addr_.toStdString();
    uv_ip4_addr(tmpAddr.c_str(), port_, &addr);

    uv_tcp_bind(tcp_socket_.get(), (const struct sockaddr *) &addr, 0);
    r = uv_listen((uv_stream_t *) tcp_socket_.get(), 128, onNewConnection);
    if (r) {
        emit sigError(addr_, port_, uv_err_name(r));
        return;
    }
    qDebug() << tr("Server listen on on %1:%2").arg(addr_).arg(port_);
}

void UvTcpServer::allocBuffer(uv_handle_t *, size_t suggested_size, uv_buf_t *buf)
{
    static char slab[65536];
    buf->base = slab;
    buf->len = suggested_size;
}
