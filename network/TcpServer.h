//
// Created by Shinan on 2019/2/3.
//

#ifndef SEVICEFRAMEWORK_TCPSERVER_H
#define SEVICEFRAMEWORK_TCPSERVER_H

#include <memory>
#include <vector>
#include <logging.h>
#include "LibeventReactor.h"
#include "ThreadPool.h"

#define MAX_CLIENTS 50000 //限制最大连接数
using namespace std;

class TcpServer
{
public:
  TcpServer(int eventReactorNum, int threadPoolNum, unsigned short listenPort);
  ~TcpServer();
  void start();
private:
  bool _initEventReactors(int reactorNum);
  bool _initThreadPool(int threadNum);
private:
  int eventReactorNum_;
  vector<shared_ptr<EventReactor>> vecSpReactor_;
  shared_ptr<ThreadPool> spThreadPool_;
  int threadPoolNum_;
  unsigned short listenPort_;
};

#endif //SEVICEFRAMEWORK_TCPSERVER_H
