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
#include "NetService.h"
#include "../network/LibeventReactor.h"

NetService::NetService(const string &name) : InteractiveTask(name)
{}
NetService::~NetService()
{}

bool NetService::_onStart()
{
  shared_ptr<EventReactor> spEventReactor(new LibeventRector);
  _setEventReactor(spEventReactor);
  LOG(INFO) << "NetService " << getTaskName() << " started!";
}

void NetService::_onStop()
{
  LOG(INFO) << "NetService " << getTaskName() << " stopped!";
}

void NetService::_onMessage(const shared_ptr<Message> &spMessage)
{
  LOG(INFO) << "msg comes, what: " << spMessage->getWhat();
}

void NetService::__onAccept(int fd)
{
  LOG(INFO) << "one client connected!";
}

