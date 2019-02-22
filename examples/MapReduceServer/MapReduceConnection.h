//
// Created by Shinan on 2019/2/18.
//

#ifndef SERVICEFRAMEWORK_MAPREDUCECONNECTION_H
#define SERVICEFRAMEWORK_MAPREDUCECONNECTION_H

#include <network/Connection.h>

class MapReduceConnection : public Connection, public enable_shared_from_this<MapReduceConnection>
{
  enum {RECV_HEAD, RECV_BODY};
public:
  MapReduceConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService);
  virtual ~MapReduceConnection();
  virtual void startReadOrWriteInService();

private:
  virtual void onMessage(const SpConnectionMessage &spConnectionMessage);
  void recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes);
  void sendCompleteCallback(int retSendStat);
  static void mapFunction(weak_ptr<MapReduceConnection> wpConnection, const string filePath);
  int reduce(int last, int cur);
  void _destroy();

private:
  bool firstCome_ = true;
  int taskCount_ = 0;
  int curMaxNum_ = 0;
  int stat_ = RECV_HEAD;

  RecvCompleteCallback recvCompleteCallback_;
  SendCompleteCallback sendCompleteCallback_;
};

#endif //SERVICEFRAMEWORK_MAPREDUCECONNECTION_H
