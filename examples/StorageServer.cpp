//
// Created by Shinan on 2019/2/3.
//

#include <network/TcpServer.h>

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