//
// Created by Shinan on 2019/2/23.
// 模拟客户端请求，和ConcurrenceEchoServer配合做并发测试
// 注意设置最大文件句柄数,先启动echo server,再启动simulate server
//测试并发连接数

//并发客户端数目
#define CLIENT_NUM  20000

//每个客户端发的消息数目
#define MSG_NUM_EACH_CLIENT  5

#include <logging.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <memory>
#include <network/TcpServer.h>
#include <network/Connector.h>
#include <network/Stream.h>
#include "ClientConnection.h"
#include "ConcurrenceSimulateServer.h"

#define REACTOR_NUM 0 //只用accept线程
#define THREAD_POOL_NUM 0 //不要线程池
#define LISTEN_PORT 1935 //随意，不需要


ConcurrenceSimulateServer::ConcurrenceSimulateServer(int netWorkServiceNum, unsigned short listenPort, int threadPoolNum)
    : TcpServer(netWorkServiceNum, listenPort, threadPoolNum), clientNum_(CLIENT_NUM), msgNum_(MSG_NUM_EACH_CLIENT)
{}

void ConcurrenceSimulateServer::_creatConnections(int num)
{
  for (int i = 0; i < clientNum_; i++)
  {
    auto spConnector = make_shared<Connector>(getSpNetAcceptService()->getSpEventReactor());
    spConnector->asyncConnect("0.0.0.0",
                              12306,
                              std::bind(&ConcurrenceSimulateServer::onConnected, this, std::placeholders::_1));
  }
}

void ConcurrenceSimulateServer::onConnected(int fd)
{
  if (fd <= 0) return;
  auto spStream = make_shared<Stream>(fd, getSpNetAcceptService()->getSpEventReactor());
  auto spClientConnection = make_shared<ClientConnection>(spStream, *this);
  connectionMap_.emplace(std::make_pair(spClientConnection->getHandle(), spClientConnection));
  spClientConnection->startReadOrWriteInService();
}

void ConcurrenceSimulateServer::onConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService)
{}
//开始前需要执行的函数
void ConcurrenceSimulateServer::onLoop()
{
  _creatConnections(clientNum_);
}

int initLogging()
{
  google::InitGoogleLogging("ConcurrenceSimulateServer.log"); //初始化glog库
  FLAGS_stderrthreshold = google::INFO;     //
  mkdir("log", 0755);
  FLAGS_log_dir = "./log";
  FLAGS_max_log_size = 4; //每个日志文件4M
}

int main()
{
  signal(SIGPIPE, SIG_IGN);//忽略pipe破裂信号
  initLogging();

  ConcurrenceSimulateServer concurrenceSimulateServer(REACTOR_NUM, LISTEN_PORT, THREAD_POOL_NUM);
  concurrenceSimulateServer.start();

  LOG(ERROR) << "concurrenceSimulateServer exit!";

  google::ShutdownGoogleLogging();
  return 0;
}