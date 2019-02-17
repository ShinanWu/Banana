//
// Created by Shinan on 2019/2/3.
//
#include <assert.h>
#include "TcpServer.h"
#include "LibeventReactor.h"
#include "MessageCenter.h"
#include "ThreadPool.h"

TcpServer::TcpServer(int netWorkServiceNum, unsigned short listenPort, int threadPoolNum)
    : netWorkServiceNum_(netWorkServiceNum),
      threadPoolNum_(threadPoolNum),
      listenPort_(listenPort)
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
  ThreadPool::initInstance(allNum, 1000);
  return true;
}

bool TcpServer::_startNetAcceptService()
{
  shared_ptr<EventReactor> spReactor(new LibeventRector);
  upNetAcceptService_.reset(new NetAcceptService("AcceptService", spReactor));

  //等待worker线程准备完毕再开始accept，否则可能起初的连接失败
  for (auto it : vecSpNetWorkService_)
  {
    weak_ptr<NetWorkService> wpNetWorkService;
    MessageCenter::Instance()->waitGetTaskRef(it->getTaskName(), wpNetWorkService);
  }
  upNetAcceptService_->start();
  return true;
}

bool TcpServer::_startNetWorkService()
{
  stringstream ss;
  for (int i = 0; i < netWorkServiceNum_; i++)
  {
    ss.str("");
    ss << "WorkService-" << i;
    shared_ptr<EventReactor> spReactor(new LibeventRector);
    vecSpNetWorkService_.emplace_back(shared_ptr<NetWorkService>(new NetWorkService(ss.str(), spReactor)));
    ThreadPool::getInstance()->syncPostTask(vecSpNetWorkService_[i]);
  }
}
