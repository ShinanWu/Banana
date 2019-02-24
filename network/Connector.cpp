//
// Created by Shinan on 2019/2/24.
//

#include "Connector.h"
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

//绑定的本地端口
struct sockaddr_in Connector::addr_in_local
    {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("0.0.0.0"),
        .sin_port = htons(12306)
    };

Connector::Connector(const SpEventReactor &spEventReactor_) : spEventReactor_(spEventReactor_)
{}

void Connector::asyncConnect(const string &ip, unsigned short port, const ConnectCallback &connectCallback)
{
  if (ip.size() < 7 || ip.size() > 15) return false;

  int ret = CONNECT_ERR;
  int sockFd = -1;
  struct sockaddr_in addr_in;
  addr_in.sin_family = AF_INET;
  addr_in.sin_addr.s_addr = inet_addr(ip.c_str());
  addr_in.sin_port = htons(port);
  int reuse_addr = 1;
  connectEventCallback_ =
      std::bind(&Connector::_onConnectEvent, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
  connectCallback_ = connectCallback;

  do
  {
    sockFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockFd < 0)
    {
      LOG(ERROR) << "creat socket failed! ret " << sockFd << " errno " << errno;
      ret = CONNECT_ERR;
      break;
    }
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, (const void *) &reuse_addr, sizeof(int)))
    {
      LOG(ERROR) << "setsockopt SO_REUSEADDR failed! errno " << errno;
      ret = CONNECT_ERR;
      break;
    }
    if (bind(sockFd, static_cast<struct sockaddr *>(&addr_in_local), sizeof(sockaddr)) == -1)
    {
      LOG(ERROR) << "bind port " << ntohs(addr_in_local.sin_port) << "failed! errno " << errno;
      ret = CONNECT_ERR;
      break;
    }
    int retVal = connect(sockFd, static_cast<struct sockaddr *>(&addr_in), sizeof(sockaddr));
    if (retVal == 0)
    {
      ret = CONNECT_OK;
      break;
    }
    if (retVal == -1)
    {
      if (errno == EINPROGRESS)
      {
        ret = CONNECTING;
        break;
      }
      else
      {
        ret = CONNECT_ERR;
        break;
      }
    }
  } while (0);

  if (ret == CONNECT_ERR)
  {
    close(sockFd);
    connectCallback_(-1);
    _destroy();
    return;
  }
  else if (ret == CONNECTING)
  {
    //仅需要判断可写事件，不用判断可读？
    spEventReactor_->addEventHandler(sockFd, EventReactor::EVENT_WRITE, connectEventCallback_);
    return;
  }
    //成功建立连接
  else
  {
    connectCallback_(sockFd);
    _destroy();
    return;
  }
}

void Connector::_onConnectEvent(int fd, short event)
{
  int err = 0;
  int len = sizeof(err);

  if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *) &err, &len) == -1)
  {
    err = -1;
  }
  if (err != 0)
  {
    LOG(ERROR) << "connect failed!";
    close(fd);
    fd = -1;
  }
  connectCallback_(fd);
  _destroy();
}

void Connector::_destroy()
{
  connectEventCallback_ = nullptr;
}
