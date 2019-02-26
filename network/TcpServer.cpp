//
// Created by Shinan on 2019/2/3.
//
#include "TcpServer.h"
#include <assert.h>
#include <functional>
#include "LibeventReactor.h"
#include "../multi-threading/MessageCenter.h"
#include "../multi-threading/ThreadPool.h"

using namespace std::placeholders;

TcpServer::TcpServer(int netWorkServiceNum, unsigned short listenPort, int threadPoolNum)
    : netWorkServiceNum_(netWorkServiceNum),
      listenPort_(listenPort),
      threadPoolNum_(threadPoolNum)
{}

TcpServer::~TcpServer()
{}

void TcpServer::start()
{
  _startThreadPool();
  _startNetWorkService();
  _startNetAcceptService();
}

bool TcpServer::_startThreadPool()
{
  assert(threadPoolNum_ >= 0);
  int allNum = threadPoolNum_ + netWorkServiceNum_;
  ThreadPool::initInstance(allNum, 1000)->start();
  return true;
}

bool TcpServer::_startNetAcceptService()
{
  SpEventReactor spReactor(new LibeventRector);
  spReactor->bindPort(listenPort_);
  spNetAcceptService_.reset(new NetAcceptService("AcceptService", spReactor, *this));
  spNetAcceptService_->vecSpWorkService_ = std::move(vecSpNetWorkService_);
  //等待worker线程准备完毕再开始accept，否则可能起初的连接失败
  for (auto& it : spNetAcceptService_->vecSpWorkService_)
  {
    weak_ptr<InteractiveTask> wpNetWorkService;
    MessageCenter::Instance()->waitGetTaskRef(it->getTaskName(), wpNetWorkService);
  }
  spNetAcceptService_->start();
  return true;
}

bool TcpServer::_startNetWorkService()
{
  stringstream ss;
  for (int i = 0; i < netWorkServiceNum_; i++)
  {
    ss.str("");
    ss << "WorkService-" << i;
    SpEventReactor spReactor(new LibeventRector);
    shared_ptr<NetWorkService> spService(new NetWorkService(ss.str(), spReactor));
    spService->setNewConnectionCallback(std::bind(&TcpServer::onConnection, this, _1, _2));
    vecSpNetWorkService_.emplace_back(spService);
    ThreadPool::getInstance()->syncPostTask(vecSpNetWorkService_[i]);
  }
}

const shared_ptr<NetAcceptService> &TcpServer::getSpNetAcceptService() const
{
  return spNetAcceptService_;
}

void TcpServer::onLoop()
{}
