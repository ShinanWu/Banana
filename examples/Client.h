//
// Created by Shinan on 2019/1/23.
//

#ifndef SEVICEFRAMEWORK_CLIENT_H
#define SEVICEFRAMEWORK_CLIENT_H

#include <vector>
#include <string>
#include "Message.h"

using namespace std;

class Client
{
#define DEFAULT_BUFF_LEN (1024) //1k
#define RECV_BUFF_MAX_SIZE (1024 * 1024 * 500) //500M
#define SEND_BUFF_MAX_SIZE (1024 * 1024 * 100) //100M 均需根据实际业务作调整，比如即时通信客户端多，buffer小，取较小合适值即可
  enum
  {
    RECVING, RECVHEAD, RECVBODY, RECVED, SENDING, SENDED, TOCLOSE
  };
 public:
  Client(int clientFd, const string &id);
  ~Client();
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
  string id_;
  int recvLen_;
  int needRecvLen_;
  int sendLen_;
  int needSendLen_;
  int recvStat_;
  int sendStat_;
  vector<char> recvBuf_;
  vector<char> sendBuf_;
};

#endif //SEVICEFRAMEWORK_CLIENT_H

