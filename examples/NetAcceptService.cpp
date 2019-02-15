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
#include "NetAcceptService.h"
#include "../network/LibeventReactor.h"

#define MAX_CLIENTS_ACCEPT 50000

NetAcceptService::NetAcceptService(const string &name, const shared_ptr<EventReactor> &spEventReactor)
    : InteractiveTask(name, spEventReactor)
{}

NetAcceptService::~NetAcceptService()
{}

void NetAcceptService::onStart()
{
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
  LOG(INFO) << "one client connected! fd: " << fd;
}



