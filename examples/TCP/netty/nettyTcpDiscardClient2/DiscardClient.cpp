#include "DiscardClient.h"
#include <QDateTime>
#include <QDebug>
#include <unistd.h>
using namespace std;
const int sendMaxCnt = 10000 * 1024;
DiscardClient::DiscardClient(const QString &addr, quint16 port, QObject *parent)
    : QObject{parent}
    , addr_(addr)
    , port_(port)
    , socket_(new QTcpSocket(this))
    , cnt_(0)
    , startTime_(0)
    , sendData(1024, 'H')
{}

DiscardClient::~DiscardClient() {}
void DiscardClient::startRecv()
{
    qDebug() << "DiscardClient::startRecv in thread:" << gettid();
    socket_->setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);

    connect(socket_, &QTcpSocket::readyRead, this, &DiscardClient::onRecvData);
    connect(socket_, &QTcpSocket::connected, this, &DiscardClient::onConnected);
    connect(socket_, &QTcpSocket::bytesWritten, this, &DiscardClient::onBytesWritten);
    socket_->connectToHost(addr_, port_);
}

void DiscardClient::onRecvData() {}

void DiscardClient::onConnected()
{
    qDebug() << "DiscardClient::onConnected in thread:" << gettid();
    startTime_ = QDateTime::currentMSecsSinceEpoch();
    writtenBytes_ = 0;
    socket_->write(sendData);
}

void DiscardClient::onBytesWritten(qint64 bytes)
{
    writtenBytes_ += bytes;
    if (writtenBytes_ == sendData.size()) {
        writtenBytes_ = 0;
        ++cnt_;
        if (cnt_ < sendMaxCnt) {
            socket_->write(sendData);
        } else {
            qint64 endTime = QDateTime::currentMSecsSinceEpoch();
            qDebug() << tr("%1:%2 spend time:%3")
                            .arg(socket_->localAddress().toString())
                            .arg(socket_->localPort())
                            .arg((endTime - startTime_) / 1000.0);
        }
    }
}
