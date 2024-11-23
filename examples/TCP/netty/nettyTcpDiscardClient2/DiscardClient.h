#ifndef DISCARDCLIENT_H
#define DISCARDCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <memory>
// RFC 862

class DiscardClient : public QObject
{
    Q_OBJECT
public:
    explicit DiscardClient(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~DiscardClient();

public slots:
    void startRecv();
signals:
private slots:
    void onRecvData();
    void onConnected();
    void onBytesWritten(qint64 bytes);

private:
    QString addr_;
    quint16 port_;
    QTcpSocket *socket_;
    qint64 cnt_;
    qint64 startTime_;
    qint64 writtenBytes_;
    QByteArray sendData;
};
#endif // DISCARDCLIENT_H
