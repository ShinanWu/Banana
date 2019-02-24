//
// Created by Shinan on 2019/2/24.
//

#ifndef SERVICEFRAMEWORK_CONCURRENCESIMULATESERVER_H
#define SERVICEFRAMEWORK_CONCURRENCESIMULATESERVER_H
#include <memory>
#include <network/TcpServer.h>
#include <network/Connector.h>
#include <network/Stream.h>

class ConcurrenceSimulateServer : public TcpServer
{
public:
  ConcurrenceSimulateServer(int netWorkServiceNum, unsigned short listenPort, int threadPoolNum);
  void onConnected(int fd);
private:
  //Loop开始前要做的事
  virtual void onLoop();
  void _creatConnections(int num);


private:
  virtual void onConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService);

public:
  //测试并发连接数
  const int clientNum_ = 5000;
  const int msgNum_ = 5;
  int connectedCount_ = 0;
  bool bConnectedFirstCount_ = true;
  bool bConnectedFirstSend_ = true;
  std::chrono::system_clock::time_point connectedCountStartTime_;
  std::chrono::system_clock::time_point connectedCountEndTime_;
  std::chrono::system_clock::time_point connectedSendStartTime_;
  std::chrono::system_clock::time_point connectedRecvEndTime_;
};

#endif //SERVICEFRAMEWORK_CONCURRENCESIMULATESERVER_H
