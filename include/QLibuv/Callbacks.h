#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <QString>
#include <functional>
#include <uv.h>
namespace shuimo {
namespace net {
typedef std::function<void(const QString addr, quint16 port, QByteArray data)> RecvCallback;
typedef std::shared_ptr<uv_tcp_t> TcpConnectionPtr;
typedef std::function<void(TcpConnectionPtr)> ConnectionCallback;
} // namespace net
} // namespace shuimo
#endif // CALLBACKS_H
