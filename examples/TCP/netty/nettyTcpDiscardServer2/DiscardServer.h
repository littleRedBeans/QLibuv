#ifndef DISCARDSERVER_H
#define DISCARDSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTimer>

class DiscardServer : public QObject
{
    Q_OBJECT
public:
    explicit DiscardServer(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~DiscardServer();

public slots:
    void startRecv();
signals:
private slots:
    void onRecvData();
    void printThroughput();
    void onNewConnection();
    void onDisconnected();

private:
    QString addr_;
    quint16 port_;
    QTcpServer *server_;
    qint64 transferred_;
    qint64 receivedMessages_;
    qint64 oldCounter_;
    qint64 startTime_;
    QTimer timer_;
};

#endif // DISCARDSERVER_H
