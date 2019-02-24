//
// Created by Shinan on 2019/2/23.
//主动建立连接的管理器，caller必须使用shared_ptr创建并由此实例内部管理生命周期。
//trick--利用循环引用，自己管理自己的生命周期

#ifndef SERVICEFRAMEWORK_CONNECTOR_H
#define SERVICEFRAMEWORK_CONNECTOR_H
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <memory>
#include <functional>
#include <assert.h>
#include "EventReactor.h"
#include "logging.h"

class Connector : public std::enable_shared_from_this<Connector>
{
  typedef function<void(int fd)> ConnectCallback;
  enum
  {
    CONNECT_OK, CONNECTING, CONNECT_ERR
  };
public:
  explicit Connector(const SpEventReactor &spEventReactor_);
  void asyncConnect(const string &ip, unsigned short port, const ConnectCallback &connectCallback);

private:
  void _onConnectEvent(int fd, short event);
  void _destroy();

private:
  EventReactor::EventCallback connectEventCallback_;
  ConnectCallback connectCallback_;
  SpEventReactor spEventReactor_;
  static struct sockaddr_in addr_in_local;
};

#endif //SERVICEFRAMEWORK_CONNECTOR_H
