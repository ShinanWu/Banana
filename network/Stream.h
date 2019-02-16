//
// Created by Shinan on 2019/1/23.
//

#ifndef SEVICEFRAMEWORK_CLIENT_H
#define SEVICEFRAMEWORK_CLIENT_H

#include <vector>
#include <string>
#include <memory>
#include "Message.h"

typedef enum
{
  RECVING, RECVHEAD, RECVBODY, RECVED, SENDING, SENDED, TOCLOSE //流状态
}StreamStat;
using namespace std;

class Stream
{
#define HEADER_LEN 4
#define DEFAULT_BUFF_LEN (1024) //1k
#define RECV_BUFF_MAX_SIZE (1024 * 1024 * 500) //500M
#define SEND_BUFF_MAX_SIZE (1024 * 1024 * 100) //100M 均需根据实际业务作调整，比如即时通信客户端多，buffer小，取较小合适值即可
 public:
  Stream(int fd);
  ~Stream();
  StreamStat recvOnePck();
  bool getPck(string &recvPck);
  bool setPck(const string &sendPck);
  StreamStat sendPck();
  int getFd_() const;
  void setFd_(int fd_);
  StreamStat getRecvStat_() const;
  void setRecvStat_(StreamStat stat_);
  StreamStat getSendStat_() const;
  void setSendStat_(StreamStat sendStat_);

 private:
  int fd_;
  int recvLen_;
  int needRecvLen_;
  int sendLen_;
  int needSendLen_;
  StreamStat recvStat_;
  StreamStat sendStat_;
  vector<char> recvBuf_; //接收信息的buffer
  vector<char> sendBuf_; //发送的buffer
};

#endif //SEVICEFRAMEWORK_CLIENT_H

