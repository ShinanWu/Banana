//
// Created by Shinan on 2019/1/23.
//

#ifndef SEVICEFRAMEWORK_CLIENT_H
#define SEVICEFRAMEWORK_CLIENT_H

#include <vector>
#include <string>
#include "Message.h"

using namespace std;

class Stream
{
#define DEFAULT_BUFF_LEN (1024) //1k
#define RECV_BUFF_MAX_SIZE (1024 * 1024 * 500) //500M
#define SEND_BUFF_MAX_SIZE (1024 * 1024 * 100) //100M 均需根据实际业务作调整，比如即时通信客户端多，buffer小，取较小合适值即可
  enum
  {
    RECVING, RECVHEAD, RECVBODY, RECVED, SENDING, SENDED, TOCLOSE //client状态
  };
 public:
  Stream(int fd);
  ~Stream();
  bool recvOnePck();
  bool getPck(vector<char> &recvPck);
  bool sendPck();
  bool setPck(const vector<char> &sendPck);
  int getFd_() const;
  void setFd_(int fd_);
  int getRecvStat_() const;
  void setRecvStat_(int stat_);
  int getSendStat_() const;
  void setSendStat_(int sendStat_);

 private:
  int fd_;
  int recvLen_;
  int needRecvLen_;
  int sendLen_;
  int needSendLen_;
  int recvStat_;
  int sendStat_;
  vector<char> recvBuf_; //接收信息的buffer
  vector<char> sendBuf_; //发送的buffer
};

#endif //SEVICEFRAMEWORK_CLIENT_H

