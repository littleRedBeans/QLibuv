#ifndef TIMESERVER_H
#define TIMESERVER_H

#include <QObject>
#include <memory>
// RFC 868
namespace shuimo {
namespace net {
class UvUdpSocket;
} // namespace net
} // namespace shuimo

class TimeServer : public QObject
{
    Q_OBJECT
public:
    explicit TimeServer(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~TimeServer();
    void startRecv();
signals:
private slots:
    void onRecvData(const QString addr, quint16 port, const QByteArray data);

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<shuimo::net::UvUdpSocket> socket_;
};
#endif // TIMESERVER_H
