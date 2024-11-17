#include "NetManager.h"
#include <QObject>
#include <QThread>
#include "EventDispatcherLibUv.h"
using namespace shuimo::net;
NetManager::NetManager()
    : networkThread_(std::make_unique<QThread>())
{
    networkThread_->setObjectName("network");
    networkThread_->setEventDispatcher(new EventDispatcherLibUv);
    networkThread_->start();
}

NetManager *NetManager::instance()
{
    static NetManager netManager;
    return &netManager;
}

void NetManager::moveToThread(QObject *object)
{
    object->moveToThread(networkThread_.get());
}
