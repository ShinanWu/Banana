//
// Created by Shinan on 2019/2/18.
//

#ifndef SERVICEFRAMEWORK_MAPREDUCECONNECTION_H
#define SERVICEFRAMEWORK_MAPREDUCECONNECTION_H

#include <network/Connection.h>

template<class Ret, class Ele>
vector<Ret> map(function<Ret(Ele)> fun, vector<Ele> vec)
{
  vector<Ret> vecRet;
  for (auto it : vec)
  {
    vecRet.emplace_back(fun(it));
  }
  return vecRet;
}

class MapReduceConnection : public Connection, public enable_shared_from_this<MapReduceConnection>
{
public:
  MapReduceConnection(const SpStream &spStream, const SpNetWorkService &spNetWorkService);
  virtual ~MapReduceConnection();
  virtual void startReadOrWriteInService();

private:
  virtual void onMessage(const SpConnectionMessage &spConnectionMessage);
  void recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes);
  void sendCompleteCallback(int retSendStat);
  void _destroy();

private:
  RecvCompleteCallback recvCompleteCallback_;
  SendCompleteCallback sendCompleteCallback_;
};

#endif //SERVICEFRAMEWORK_MAPREDUCECONNECTION_H
