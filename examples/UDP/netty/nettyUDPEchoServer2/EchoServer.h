#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <memory>
// RFC 862

class EchoServer : public QObject
{
    Q_OBJECT
public:
    explicit EchoServer(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~EchoServer();

public slots:
    void startRecv();
signals:
private slots:
    void onRecvData();
    void printThroughput();

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<QUdpSocket> socket_;
    qint64 transferred_;
    qint64 receivedMessages_;
    qint64 oldCounter_;
    qint64 startTime_;
    QTimer timer_;
};
#endif // ECHOSERVER_H
