#ifndef DISCARDSERVER_H
#define DISCARDSERVER_H

#include <QObject>
#include <QTimer>
#include <atomic>
#include <memory>

namespace shuimo {
namespace net {
class UvTcpServer;
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
    void printThroughput();

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<shuimo::net::UvTcpServer> socket_;
    std::atomic_int64_t transferred_;
    std::atomic_int64_t receivedMessages_;
    qint64 oldCounter_;
    qint64 startTime_;
    QTimer timer_;
};

#endif // DISCARDSERVER_H
