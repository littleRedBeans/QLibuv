#ifndef DAYTIMESERVER_H
#define DAYTIMESERVER_H

#include <QObject>
#include <memory>
// RFC 867
namespace shuimo {
namespace net {
class UvUdpSocket;
} // namespace net
} // namespace shuimo

class DaytimeServer : public QObject
{
    Q_OBJECT
public:
    explicit DaytimeServer(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~DaytimeServer();
    void startRecv();
signals:
private slots:
    void onRecvData(const QString addr, quint16 port, const QByteArray data);

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<shuimo::net::UvUdpSocket> socket_;
};
#endif // DAYTIMESERVER_H
