#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <memory>
// RFC 862

class EchoClient : public QObject
{
    Q_OBJECT
public:
    explicit EchoClient(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~EchoClient();

public slots:
    void startRecv();
signals:
private slots:
    void onRecvData();

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<QUdpSocket> socket_;
};
#endif // ECHOCLIENT_H
