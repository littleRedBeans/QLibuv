#ifndef DISCARDCLIENT_H
#define DISCARDCLIENT_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <memory>

class DiscardClient : public QObject
{
    Q_OBJECT
public:
    explicit DiscardClient(const QString &addr, quint16 port, QObject *parent = nullptr);
    ~DiscardClient();

public slots:
    void startRecv();
signals:

private:
    QString addr_;
    quint16 port_;
    std::unique_ptr<QUdpSocket> socket_;
};
#endif // DISCARDCLIENT_H
