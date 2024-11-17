#ifndef NETMANAGER_H
#define NETMANAGER_H
#include <memory>
class QObject;
class QThread;
namespace shuimo {
namespace net {
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
