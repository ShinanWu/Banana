//
// Created by Shinan on 2019/2/24.
//
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <logging.h>
#include "ConcurrenceEchoServer.h"
#include "ConcurrentConnection.h"

#define REACTOR_NUM 4 //和CPU数目一致
#define THREAD_POOL_NUM 0 //不要线程池
#define LISTEN_PORT 12306 //随意，不需要

ConcurrenceEchoServer::ConcurrenceEchoServer(int netWorkServiceNum, unsigned short listenPort, int threadPoolNum)
    : TcpServer(netWorkServiceNum, listenPort, threadPoolNum)
{}

void ConcurrenceEchoServer::onConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService)
{
//  LOG(INFO) << "new connection!";
  auto spConnection = make_shared<ConcurrentConnection>(spStream);
  //这里需要持有引用是因为GCC的bug使得shared_ptr在enable_shared_from_this后不能在类里析构
  auto spService = wpNetWorkService.lock();
  spService->addNewConnection(spConnection);
  spConnection->startReadOrWriteInService();
}

int initLogging()
{
  google::InitGoogleLogging("ConcurrenceEchoServer.log"); //初始化glog库
  FLAGS_stderrthreshold = google::INFO;     //
  mkdir("log", 0755);
  FLAGS_log_dir = "./log";
  FLAGS_max_log_size = 4; //每个日志文件4M
}

int main()
{
  signal(SIGPIPE, SIG_IGN);//忽略pipe破裂信号
  initLogging();

  ConcurrenceEchoServer concurrenceEchoServer(REACTOR_NUM, LISTEN_PORT, THREAD_POOL_NUM);
  concurrenceEchoServer.start();
  LOG(ERROR) << "ConcurrenceEchoServer exit!";

  google::ShutdownGoogleLogging();
  return 0;
}