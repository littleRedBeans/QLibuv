#ifndef NETMANAGER_H
#define NETMANAGER_H
#include <QString>
#include <memory>
class QObject;
class QThread;
namespace shuimo {
namespace net {
struct SendContext
{
    QString addr_;
    quint16 port_;
    QByteArray data_;
    explicit SendContext(QString addr, quint16 port, QByteArray data)
        : addr_(addr)
        , port_(port)
        , data_(data)
    {}
};
class NetManager
{
public:
    static NetManager *instance();
    void moveToThread(QObject *object);

private:
    NetManager();
    NetManager(const NetManager &) = delete;
    NetManager &operator=(const NetManager &) = delete;
    std::unique_ptr<QThread> networkThread_;
};
} // namespace net
} // namespace shuimo

#endif // NETMANAGER_H
