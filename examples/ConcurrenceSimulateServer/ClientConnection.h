//
// Created by Shinan on 2019/2/24.
//

#ifndef SERVICEFRAMEWORK_CLIENTCONNECTION_H
#define SERVICEFRAMEWORK_CLIENTCONNECTION_H
#include <network/Connection.h>
#include <memory>
#include <network/NetWorkService.h>

class ConcurrenceSimulateServer;
class ClientConnection : public Connection, public enable_shared_from_this<ClientConnection>
{
  enum
  {
    RECV_HEAD, RECV_BODY, SEND_HEAD, SENDBODY
  };
public:
  ClientConnection(const SpStream &spStream,
                   const WpNetWorkService &wpNetWorkService,
                   ConcurrenceSimulateServer &simulateServer);
public:
  virtual void startReadOrWriteInService();

private:
  void _destroy();
  void recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes);
  void sendCompleteCallback(int retSendStat);

private:
  int stat_ = RECV_HEAD;
  stringstream ss_;
  int selfEchoCount_ = 0;
  vector<char> sendBuf_;
  vector<char> recvBuf_;
  RecvCompleteCallback recvCompleteCallback_;
  SendCompleteCallback sendCompleteCallback_;
  ConcurrenceSimulateServer &simulateServer_;

};

#endif //SERVICEFRAMEWORK_CLIENTCONNECTION_H
