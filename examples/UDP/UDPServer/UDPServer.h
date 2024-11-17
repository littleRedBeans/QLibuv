#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <memory>

namespace shuimo {
namespace net {
class UvUdpSocket;
} // namespace net
} // namespace shuimo
class UDPServer : public QObject
{
    Q_OBJECT
public:
    UDPServer(QString addr, quint16 port, QObject *parent = nullptr);
    ~UDPServer();
    void startRecv();

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<shuimo::net::UvUdpSocket> socket_;
};
#endif // UDPSERVER_H
