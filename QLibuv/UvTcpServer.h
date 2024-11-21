#ifndef UVTCPSERVER_H
#define UVTCPSERVER_H

#include <QObject>
#include "Callbacks.h"
#include <atomic>
#include <functional>
#include <memory>
namespace shuimo {
namespace net {

class Q_DECL_EXPORT UvTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit UvTcpServer(const QString &addr,
                         quint16 port,
                         RecvCallback recvCb = nullptr,
                         QObject *parent = nullptr);
    ~UvTcpServer();
    void sendData(QByteArray &data, const QString &address, quint16 port);
    void startListen();
    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
    void setRecvCallback(const RecvCallback &cb) { recvCallback_ = cb; }
    QString addr() const { return addr_; }
    quint16 port() const { return port_; }
signals:
    void sigRecv(const QString addr, quint16 port, QByteArray data);
    void sigError(const QString addr, quint16 port, const QString err);
    void sigSendFail(const QString addr, quint16 port, QByteArray data, const QString err);
    void sigRecvFail(const QString addr, quint16 port, const QString err);

private:
    Q_INVOKABLE void invokeStartListen();
    Q_INVOKABLE void invokeSendData(QByteArray data, const QString &address, quint16 port);
    static void allocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void onNewConnection(uv_stream_t *server, int status);
    static void onRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    Q_DISABLE_COPY(UvTcpServer) //noncopyable UvTcpServer
    const QString addr_;
    const quint16 port_;
    RecvCallback recvCb_;
    std::atomic_bool start_;
    uv_loop_t *loop_;
    std::unique_ptr<uv_tcp_t> tcp_socket_;
    ConnectionCallback connectionCallback_;
    RecvCallback recvCallback_;
};
} // namespace net
} // namespace shuimo
#endif // UVTCPSERVER_H
