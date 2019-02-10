//
// Created by Shinan on 2019/2/3.
//
#include <memory>
#include <network/TcpServer.h>

#define REACTOR_NUM 4 //根据CPU数目来，一般可设为CPU数目相当
#define THREAD_POOL_NUM 50 //根据测试进行调整，值不要太大
#define LISTEN_PORT 12306

class StorageServer : public TcpServer
{
public:
  StorageServer(int eventReactorNum, int threadPoolNum, unsigned short listenPort)
      :TcpServer(eventReactorNum, threadPoolNum, listenPort)
  {}
  void start()
  {
    LOG(INFO) << "StorageServer started!";
    TcpServer::start();
  }

};

int main()
{
  StorageServer storageServer(REACTOR_NUM, THREAD_POOL_NUM, LISTEN_PORT);
  storageServer.start();
  LOG(ERROR) << "Storage Server stoped!";
}