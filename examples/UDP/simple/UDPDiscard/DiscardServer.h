#ifndef DISCARDSERVER_H
#define DISCARDSERVER_H

#include <QObject>
#include <memory>
// RFC 863
namespace shuimo {
namespace net {
class UvUdpSocket;
} // namespace net
} // namespace shuimo

class DiscardServer : public QObject
{
    Q_OBJECT
public:
    explicit DiscardServer(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~DiscardServer();
    void startRecv();
signals:
private slots:
    void onRecvData(const QString addr, quint16 port, QByteArray data);

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<shuimo::net::UvUdpSocket> socket_;
};

#endif // DISCARDSERVER_H
