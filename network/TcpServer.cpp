//
// Created by Shinan on 2019/2/3.
//

#include "TcpServer.h"
TcpServer::TcpServer(int eventReactorNum, int threadPoolNum, unsigned short listenPort)
    : eventReactorNum_(eventReactorNum),
      threadPoolNum_(threadPoolNum),
      listenPort_(listenPort)
{}
TcpServer::~TcpServer()
{}
void TcpServer::start()
{
  _initThreadPool(threadPoolNum_);
  _initEventReactors(eventReactorNum_);

}
bool TcpServer::_initEventReactors(int reactorNum)
{
  assert(reactorNum > 0 && reactorNum < threadPoolNum_);
  vector<shared_ptr<EventReactor>> vecSpReactor(reactorNum);
  for (auto &spReactor : vecSpReactor)
  {
    spReactor = new LibeventRector();
    if (!spReactor->initReactor(MAX_CLIENTS / (reactorNum - 1))) //减去一条accept线程
    {
      LOG(ERROR) << "onStart LibeventRector failed!";
      return false;
    }
  }
  LOG(INFO) << "onStart Reactors success! num:" << reactorNum;
  return true;

}
bool TcpServer::_initThreadPool(int threadNum)
{
  assert(threadNum > 0); //线程池数目应该大于0
  spThreadPool_ = new ThreadPool(threadNum);
  return false;
}
