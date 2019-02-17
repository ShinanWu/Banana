//
// Created by Shinan on 2019/1/22.
//创建一个libevent集成的网络服务
//功能是网络通信并生成IndependentTask和InteractiveTask，并和InteractiveTask通信
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <logging.h>
#include <assert.h>
#include <memory>
#include "NetAcceptService.h"
#include "LibeventReactor.h"
#include "NewConnectMessage.h"

#define MAX_CLIENTS_ACCEPT 50000
using namespace std;
using namespace std::placeholders;

NetAcceptService::NetAcceptService(const string &name, const SpEventReactor &spEventReactor)
    : InteractiveTask(name, spEventReactor)
{
  assert(spEventReactor);
}

NetAcceptService::~NetAcceptService()
{}

void NetAcceptService::onStart()
{
  spEventReactor_->addEventHandler(NULL,
                                   EventReactor::EVENT_ACCEPT,
                                   std::bind(&NetAcceptService::__onAccept, this, _1));
  LOG(INFO) << "NetAcceptService " << getTaskName() << " started!";
}

void NetAcceptService::onStop()
{
  LOG(INFO) << "NetAcceptService " << getTaskName() << " stopped!";
}

void NetAcceptService::onMessage(const shared_ptr<Message> &spMessage)
{
  LOG(INFO) << "msg comes, what: " << spMessage->getWhat();
}

void NetAcceptService::__onAccept(int fd)
{
  //Round-Robin向每个worker发送
  auto spMessage = std::make_shared<NewConnectMessage>(fd);
  vecSpWorkService_[_nextServiceIndex()]->notifyMsg(spMessage);
  LOG(INFO) << "one client connected! fd: " << fd;
}

int NetAcceptService::_nextServiceIndex()
{
  curServiceIndex_++;
  if (curServiceIndex_ == vecSpWorkService_.size())
  { curServiceIndex_ = 0; }
  return curServiceIndex_;
}



