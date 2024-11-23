#include "DiscardServer.h"
#include <QDateTime>
#include <QDebug>
#include <QTcpSocket>
#include <unistd.h>
using namespace std;
DiscardServer::DiscardServer(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , server_(new QTcpServer(this))
    , transferred_(0)
    , receivedMessages_(0)
    , oldCounter_(0)
    , startTime_(0)
    , timer_(this)
{}

DiscardServer::~DiscardServer()
{
    // 关闭服务器时，如果有正在连接的客户端，关闭这些连接
    if (server_->isListening()) {
        server_->close();
    }
}
void DiscardServer::startRecv()
{
    // 监听本地的8888端口，可根据需求修改端口号
    if (server_->listen(QHostAddress(addr_), port_)) {
        qDebug() << tr("Server started, listening on %1:%2").arg(addr_).arg(port_);
        connect(server_, &QTcpServer::newConnection, this, &DiscardServer::onNewConnection);
    } else {
        qDebug() << "Server failed to start: " << server_->errorString();
    }
    startTime_ = QDateTime::currentMSecsSinceEpoch();
    connect(&timer_, &QTimer::timeout, this, &DiscardServer::printThroughput);
    timer_.start(3000);
}
void DiscardServer::printThroughput()
{
    qint64 endTime = QDateTime::currentMSecsSinceEpoch();
    qint64 bytes = transferred_ - oldCounter_;
    double time = (endTime - startTime_) / 1000.0;
    qDebug("%4.3f MiB/s %4.3f Ki Msg/s %6.2f bytes per msg,total recv %4.3f MiB\n",
           static_cast<double>(bytes) / time / 1024 / 1024,
           static_cast<double>(receivedMessages_) / time / 1024,
           static_cast<double>(bytes) / static_cast<double>(receivedMessages_),
           static_cast<double>(transferred_) / 1024 / 1024);
    receivedMessages_ = 0;
    oldCounter_ = transferred_;
    startTime_ = endTime;
}

void DiscardServer::onNewConnection()
{
    QTcpSocket *clientSocket = server_->nextPendingConnection();
    if (clientSocket) {
        qDebug() << "New client connected: " << clientSocket->peerAddress().toString() << ":"
                 << clientSocket->peerPort();
        connect(clientSocket, &QTcpSocket::readyRead, this, &DiscardServer::onRecvData);
        connect(clientSocket, &QTcpSocket::disconnected, this, &DiscardServer::onDisconnected);
    }
}

void DiscardServer::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {
        qDebug() << "Client disconnected: " << socket->peerAddress().toString() << ":"
                 << socket->peerPort();
        socket->deleteLater();
    }
}

void DiscardServer::onRecvData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {
        QByteArray data = socket->readAll();
        ++receivedMessages_;
        transferred_ += data.size();
    }
}
