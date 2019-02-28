//
// Created by Shinan on 2019/2/23.
// 模拟客户端请求，和ConcurrenceEchoServer配合做并发测试
// 注意设置最大文件句柄数(100000),
// 注意增大可用端口范围 -- 修改/etc/sysctl.conf，增加一行：net.ipv4.ip_local_port_range= 1024 65535
// 这样可以测试5W多并发
// 先启动echo server,再启动simulate server,
// 假设4个核，每个服务占用两个，粗略并发量x2

//测试并发连接数

//并发客户端数目
#define CLIENT_NUM  40000

//每个客户端发的消息数目
#define MSG_NUM_EACH_CLIENT  20

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
#include <network/NewConnectMessage.h>
#include <chrono>


#define REACTOR_NUM 2 //模拟客户端用两个线程，占用2个核，服务端用其余两个核
#define THREAD_POOL_NUM 0 //不用线程池
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

//异步connect建立起连接的每个fd
void ConcurrenceSimulateServer::onConnected(int fd)
{
  if (fd <= 0) return;

  //计算所有连接建立成功的时间
  if (bConnectedFirstCount_)
  {
    bConnectedFirstCount_ = false;
    connectedCountStartTime_ = std::chrono::system_clock::now();
    LOG(ERROR) << " first client connected! fd: " << fd;
  }
  if (++connectedCount_ == clientNum_)
  {
    connectedCountEndTime_ = std::chrono::system_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        connectedCountEndTime_ - connectedCountStartTime_).count();
    LOG(ERROR) << " all " << clientNum_ << " clients connected! duration:"
               << duration << " ms";
    google::FlushLogFiles(google::INFO);
  }
  getSpNetAcceptService()->__onAccept(fd);

}

//每个NetWorkService连接来的回调函数
void ConcurrenceSimulateServer::onConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService)
{
  auto spClientConnection = make_shared<ClientConnection>(spStream, wpNetWorkService, *this);
  auto spNetworkService = wpNetWorkService.lock();
  assert(spNetworkService);
  spNetworkService->addNewConnection(spClientConnection);
  spClientConnection->startReadOrWriteInService();
}
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