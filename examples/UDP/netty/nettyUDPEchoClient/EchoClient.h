#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QObject>
#include <memory>
// RFC 862
namespace shuimo {
namespace net {
class UvUdpSocket;
} // namespace net
} // namespace shuimo

class EchoClient : public QObject
{
    Q_OBJECT
public:
    explicit EchoClient(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~EchoClient();
    void startRecv();
public slots:
    void sendData();
signals:
private slots:
    void onRecvData(const QString addr, quint16 port, QByteArray data);

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<shuimo::net::UvUdpSocket> socket_;
};
#endif // ECHOCLIENT_H
