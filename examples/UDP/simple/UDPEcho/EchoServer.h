#ifndef ECHOSERVER_H
#define ECHOSERVER_H
#include <QObject>
#include <memory>
// RFC 862
namespace shuimo {
namespace net {
class UvUdpSocket;
} // namespace net
} // namespace shuimo

class EchoServer : public QObject
{
    Q_OBJECT
public:
    explicit EchoServer(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~EchoServer();
    void startRecv();
signals:
private slots:
    void onRecvData(const QString addr, quint16 port, const QByteArray data);

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<shuimo::net::UvUdpSocket> socket_;
};
#endif // ECHOSERVER_H
