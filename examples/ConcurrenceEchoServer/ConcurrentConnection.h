//
// Created by Shinan on 2019/2/24.
//

#ifndef SERVICEFRAMEWORK_CONCURRENTCONNECTION_H
#define SERVICEFRAMEWORK_CONCURRENTCONNECTION_H

#include <network/Connection.h>

class ConcurrentConnection : public Connection, public enable_shared_from_this<ConcurrentConnection>
{
  enum {RECV_HEAD, RECV_BODY};
public:
  ConcurrentConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService);
  virtual void startReadOrWriteInService() final;

private:
  void recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes);
  void sendCompleteCallback(int retSendStat);
  void _destroy();

private:
  int stat_ = RECV_HEAD;
  vector<char> recvBuf_;
  RecvCompleteCallback recvCompleteCallback_;
  SendCompleteCallback sendCompleteCallback_;
};

#endif //SERVICEFRAMEWORK_CONCURRENTCONNECTION_H
