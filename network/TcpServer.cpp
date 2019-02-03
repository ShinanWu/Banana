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

}
bool TcpServer::initEventReactors(int reactorNum)
{
  assert(reactorNum > 0);
  vector<shared_ptr<EventReactor>> vecSpReactor(reactorNum);
  for (auto &spReactor : vecSpReactor)
  {
    spReactor = new LibeventRector();
    if (!spReactor->initReactor(MAX_CLIENTS / (reactorNum - 1))) //减去一条accept线程
    {
      LOG(ERROR) << "init LibeventRector failed!";
      return false;
    }
  }
  LOG(INFO) << "init Reactors success! num:" << reactorNum;
  return true;

}
bool TcpServer::initThreadPool(int threadNum)
{
  assert(threadNum >= eventReactorNum_); //线程池数目应该大于reactor数目
  spThreadPool_ = new ThreadPool(threadNum);
  return false;
}
