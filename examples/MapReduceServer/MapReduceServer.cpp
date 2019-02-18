//
// Created by Shinan on 2019/2/18.
//
//以简单的map-reduce为例，实现CPU并行计算
//从10个文件里找到最大的数并返回给客户端

#include <memory>
#include <network/TcpServer.h>
#include <iostream>
#include <logging.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include "multi-threading/ThreadPool.h"
#include <network/LibeventReactor.h>
#include "network/NetAcceptService.h"
#include "network/NetWorkService.h"
#include "MapReduceConnection.h"

#define REACTOR_NUM 4 //根据CPU数目来，一般可设为CPU数目相当
#define THREAD_POOL_NUM 16 //根据测试进行调整，值不要太大
#define LISTEN_PORT 10087

using namespace std;

class MapReduceServer : public TcpServer
{
public:
  MapReduceServer(int eventReactorNum, unsigned short listenPort, int threadPoolNum)
      : TcpServer(eventReactorNum, listenPort, threadPoolNum)
  {}

private:
  void onConnection(const SpStream &spStream, const SpNetWorkService &spNetWorkService)
  {
    LOG(INFO) << "new connection!";
    auto spConnection = make_shared<MapReduceConnection>(spStream, spNetWorkService);
    assert(spNetWorkService->addNewConnection(spConnection));
    spConnection->startReadOrWriteInService();
  }
};

int initLogging()
{
  google::InitGoogleLogging("MapReduceServer.log"); //初始化glog库
  FLAGS_stderrthreshold = google::INFO;     //
  mkdir("log", 0755);
  FLAGS_log_dir = "./log";
  FLAGS_max_log_size = 4; //每个日志文件4M
}

int main()
{
  signal(SIGPIPE, SIG_IGN);//忽略pipe破裂信号
  initLogging();
  MapReduceServer mapReduceServer(REACTOR_NUM, LISTEN_PORT, THREAD_POOL_NUM);
  mapReduceServer.start();
  LOG(ERROR) << "MapReduceServer exit!";

  google::ShutdownGoogleLogging();
  return 0;
}