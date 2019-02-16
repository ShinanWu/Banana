//
// Created by Shinan on 2019/2/3.
//
#include <memory>
#include <network/TcpServer.h>
#include <iostream>
#include <logging.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include "ThreadPool.h"
#include "StorageService.h"
#include "network/NetAcceptService.h"
#include "network/NetWorkService.h"

#define REACTOR_NUM 4 //根据CPU数目来，一般可设为CPU数目相当
#define THREAD_POOL_NUM 50 //根据测试进行调整，值不要太大
#define LISTEN_PORT 12306

using namespace std;

class StorageServer : public TcpServer
{
public:
  StorageServer(int eventReactorNum, int threadPoolNum, unsigned short listenPort)
      :TcpServer(eventReactorNum, listenPort)
  {}
  void start()
  {
    LOG(INFO) << "StorageServer started!";
    TcpServer::start();
  }

};


int initLogging()
{
  google::InitGoogleLogging("ServiceFramework.log"); //初始化glog库
  FLAGS_stderrthreshold = google::INFO;     //
  mkdir("log", 0755);
  FLAGS_log_dir = "./log";
  FLAGS_max_log_size = 4; //每个日志文件4M
}


int main()
{
  signal(SIGPIPE, SIG_IGN);//忽略pipe破裂信号
  initLogging();
  //启动线程池
  ThreadPool::initInstance(50)->start();
  shared_ptr<EventReactor> spEventReactor(new LibeventRector);
  shared_ptr<NetAcceptService> spNetAcceptService(new NetAcceptService("NetListen", spEventReactor));
  for(int count = 0; count < 50; count++)
  {
    stringstream ss;
    ss << "Service-" << count;
    shared_ptr<Task> spTask(new StorageService(ss.str()));
    ThreadPool::getInstance()->syncPostTask(spTask);
  }
  spNetAcceptService->start(); //主线程accept
  google::ShutdownGoogleLogging();
  return 0;
}