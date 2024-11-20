#ifndef MULTICASTSERVER_H
#define MULTICASTSERVER_H

#include <QObject>
#include <memory>

namespace shuimo {
namespace net {
class UvUdpSocket;
} // namespace net
} // namespace shuimo

class MultiCastServer : public QObject
{
    Q_OBJECT
public:
    explicit MultiCastServer(const QString &bindAddr,
                             const QString &interfaceAddr,
                             quint16 port,
                             QObject *parent = nullptr);
    ~MultiCastServer();
    void startRecv(QStringList multicastAddrs);
signals:
private slots:
    void onRecvData(const QString addr, quint16 port, QByteArray data);

private:
    QString addr_;
    QString interfaceAddr_;
    quint16 port_;
    std::unique_ptr<shuimo::net::UvUdpSocket> socket_;
};
#endif // MULTICASTSERVER_H
