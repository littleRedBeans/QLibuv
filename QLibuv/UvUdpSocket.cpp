#include "UvUdpSocket.h"
#include <QDebug>
#include "EventDispatcherLibUv.h"
#include "NetManager.h"
#include <unistd.h>
using namespace shuimo::net;
using namespace std;
using namespace std::placeholders;

UvUdpSocket::UvUdpSocket(const QString &ip, quint16 port, QObject *parent)
    : QObject(parent)
    , addr_(ip)
    , port_(port)
    , start_(false)
{
    NetManager::instance()->moveToThread(this);
}

UvUdpSocket::~UvUdpSocket()
{
    if (nullptr == udp_socket_) {
        return;
    }
    closeSocket();
}

void UvUdpSocket::sendData(QByteArray &data, const QString &address, quint16 port)
{
    QMetaObject::invokeMethod(this,
                              "invokeSendData",
                              Q_ARG(QByteArray, data),
                              Q_ARG(const QString &, address),
                              Q_ARG(quint16, port));
}

void UvUdpSocket::invokeSendData(QByteArray data, const QString &address, quint16 port)
{
    struct sockaddr_in addr;
    uv_ip4_addr(address.toStdString().c_str(), port, &addr);

    auto *context = new SendContext(address, port, data);

    uv_buf_t uv_buf;
    uv_buf.base = data.data();
    uv_buf.len = data.size();

    // create uv_udp_send_t and save context into it
    auto *send_req = new uv_udp_send_t;
    send_req->data = context;

    int r = uv_udp_send(send_req,
                        udp_socket_.get(),
                        &uv_buf,
                        1,
                        (const struct sockaddr *) &addr,
                        onSendComplete);
    if (r) {
        emit sigSendFail(address, port, data, uv_strerror(r));
        delete context;
        delete send_req;
    }
}
void UvUdpSocket::onSendComplete(uv_udp_send_t *req, int status)
{
    auto *context = static_cast<SendContext *>(req->data);

    if (status < 0) {
        UvUdpSocket *socket = static_cast<UvUdpSocket *>(
            uv_handle_get_data((uv_handle_t *) req->handle));
        socket->sigSendFail(context->addr_, context->port_, context->data_, uv_strerror(status));
    }
    delete context;
    delete req;
}
void UvUdpSocket::initsocket()
{
    QMetaObject::invokeMethod(this, "invokeInitSocket");
}

void UvUdpSocket::startRecv()
{
    QMetaObject::invokeMethod(this, "invokeStartRecv");
}

void UvUdpSocket::invokeStartRecv()
{
    if (nullptr == udp_socket_) {
        return;
    }
    int r = uv_udp_recv_start(udp_socket_.get(), allocBuffer, onUdpReadStatic);
    if (r) {
        emit sigError(addr_, port_, uv_err_name(r));
    }
    qDebug() << tr("start recv successed on %1:%2").arg(addr_).arg(port_);
}

void UvUdpSocket::invokeInitSocket()
{
    assert(false == start_);
    start_ = true;
    //create uv_loop_t uv_udp_t
    EventDispatcherLibUv *dispatcher = static_cast<EventDispatcherLibUv *>(
        QAbstractEventDispatcher::instance());
    loop_ = dispatcher->uvLoop();
    udp_socket_ = make_unique<uv_udp_t>();

    //set context UvUdpSocket pointer to udp_socket_
    uv_handle_set_data((uv_handle_t *) udp_socket_.get(), this);

    //bind to Ip:port
    uv_udp_init(loop_, udp_socket_.get());
    struct sockaddr_in addr;
    uv_ip4_addr(addr_.toStdString().c_str(), port_, &addr);

    int r = uv_udp_bind(udp_socket_.get(), (const struct sockaddr *) &addr, 0);
    if (r) {
        emit sigError(addr_, port_, uv_err_name(r));
    }
    qDebug() << tr("bind successed on %1:%2").arg(addr_).arg(port_);
}

void UvUdpSocket::closeSocket()
{
    uv_udp_recv_stop(udp_socket_.get());
    uv_close((uv_handle_t *) udp_socket_.get(), nullptr);
}

void UvUdpSocket::onUdpRead(uv_udp_t *,
                            ssize_t nread,
                            const uv_buf_t *buf,
                            const struct sockaddr *addr)
{
    if (nread > 0) {
        //qDebug() << "uvSocket recv in thread id" << gettid();
        char addr_str[INET6_ADDRSTRLEN];
        const struct sockaddr_in *ipv4_addr = (const struct sockaddr_in *) addr;
        quint16 port = ntohs(ipv4_addr->sin_port);
        uv_ip4_name(ipv4_addr, addr_str, sizeof(addr_str));
        QByteArray receivedData(buf->base, nread);
        emit sigRecv(addr_str, port, receivedData);
    } else if (nread < 0) {
        emit sigRecvFail(addr_, port_, uv_strerror(nread));
    }
}
void UvUdpSocket::allocBuffer(uv_handle_t *, size_t suggested_size, uv_buf_t *buf)
{
    static char slab[65536];
    buf->base = slab;
    buf->len = suggested_size;
}
void UvUdpSocket::onUdpReadStatic(
    uv_udp_s *req, long nread, const uv_buf_t *buf, const sockaddr *addr, unsigned int)
{
    UvUdpSocket *socket = static_cast<UvUdpSocket *>(uv_handle_get_data((uv_handle_t *) req));
    socket->onUdpRead((uv_udp_t *) req, (ssize_t) nread, buf, addr);
}
