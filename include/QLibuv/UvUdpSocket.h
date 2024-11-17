#ifndef UVUDPSOCKET_H
#define UVUDPSOCKET_H
#include <QObject>
#include <atomic>
#include <functional>
#include <memory>
#include <uv.h>
namespace shuimo {
namespace net {
class Q_DECL_EXPORT UvUdpSocket : public QObject
{
    Q_OBJECT
public:
    struct SendContext
    {
        QString addr_;
        quint16 port_;
        QByteArray data_;
        explicit SendContext(QString addr, quint16 port, QByteArray data)
            : addr_(addr)
            , port_(port)
            , data_(data)
        {}
    };
    typedef std::function<void(const QString addr, quint16 port, QByteArray data)> RecvCallback;
    explicit UvUdpSocket(const QString &addr,
                         quint16 port,
                         RecvCallback recvCb = nullptr,
                         QObject *parent = nullptr);
    ~UvUdpSocket();
    void sendData(QByteArray &data, const QString &address, quint16 port);
    void initsocket();
    void startRecv();
    QString addr() const { return addr_; }
    quint16 port() const { return port_; }
signals:
    void sigRecv(const QString addr, quint16 port, QByteArray data);
    void sigError(const QString addr, quint16 port, const QString err);
    void sigSendFail(const QString addr, quint16 port, QByteArray data, const QString err);
    void sigRecvFail(const QString addr, quint16 port, const QString err);

private:
    Q_INVOKABLE void invokeStartRecv();
    Q_INVOKABLE void invokeInitSocket();
    Q_INVOKABLE void invokeSendData(QByteArray data, const QString &address, quint16 port);
    void closeSocket();
    void onUdpRead(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr);
    static void allocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void onUdpReadStatic(
        uv_udp_s *req, long nread, const uv_buf_t *buf, const sockaddr *addr, unsigned int flags);
    static void onSendComplete(uv_udp_send_t *req, int status);
    Q_DISABLE_COPY(UvUdpSocket) //noncopyable UvUdpSocket
    const QString addr_;
    const quint16 port_;
    RecvCallback recvCb_;
    std::atomic_bool start_;
    uv_loop_t *loop_;
    std::unique_ptr<uv_udp_t> udp_socket_;
};
} // namespace net

} // namespace shuimo

#endif // UVUDPSOCKET_H
