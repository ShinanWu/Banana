//
// Created by Shinan on 2019/1/23.
//每个fd由stream封装，读写数据只需注册回调函数即可
//用法可看example

#ifndef SEVICEFRAMEWORK_CLIENT_H
#define SEVICEFRAMEWORK_CLIENT_H

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "multi-threading/Message.h"
#include "EventReactor.h"

using namespace std;

typedef function<void(int fd)> SocketCloseCallback;
typedef function<void(int retRecvStat, const vector<char> &vecBytes)> RecvCompleteCallback;
typedef function<void(int retSendStat)> SendCompleteCallback;
enum _StreamStat
{
  RECVING, RECVED, RECVERR, SENDING, SENDED, SENDERR, CLOSED //流状态
};

class Stream
{
#define HEADER_LEN 4
#define DEFAULT_BUFF_LEN (1024) //1k
#define RECV_BUFF_MAX_SIZE (1024 * 1024 * 500) //500M
#define SEND_BUFF_MAX_SIZE (1024 * 1024 * 100) //100M 均需根据实际业务作调整，比如即时通信客户端多，buffer小，取较小合适值即可

public:
  Stream(int fd, const SpEventReactor &spEventReactor);
  virtual ~Stream();
  bool asyncRecvBytes(int num, const RecvCompleteCallback &recvCompleteCallback);
  bool asyncSendBytes(const vector<char> &vecBytes, const SendCompleteCallback &sendCompleteCallback);
  void setOnCloseCallback_(const SocketCloseCallback &onCloseCallback_);
  //清除成员变量，以防持有其他实例的引用
  void destory();

  int getFd() const;
  int getRecvStat_() const;
  int getSendStat_() const;
  const SpEventReactor &getSpEventReactor_() const;

private:
  void _recvOnePack(int fd, short event);
  void _sendOnePack(int fd, short event);

private:
  int fd_;
  int recvLen_ = 0;
  int needRecvLen_ = 0;
  int sendLen_ = 0;
  int needSendLen_ = 0;
  int recvStat_ = RECVED;
  int sendStat_ = SENDED;
  vector<char> recvBuf_ ; //接收信息的buffer
  vector<char> sendBuf_; //发送的buffer
  function<void(int fd, short event)> recvOnePackCall_;
  function<void(int fd, short event)> sendOnePackCall_;
  SocketCloseCallback closeCallback_;
  RecvCompleteCallback recvCompleteCallback_;
  SendCompleteCallback sendCompleteCallback_;
  SpEventReactor spEventReactor_;
};
typedef shared_ptr<Stream> SpStream;

#endif //SEVICEFRAMEWORK_CLIENT_H

