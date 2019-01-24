//
// Created by Shinan on 2019/1/23.
//
//每一个连接的客户端生成一个client对象

#include <unistd.h>
#include "Client.h"
#include "Message.h"
#include <errno.h>
#include <logging.h>
#include <string.h>

Client::Client(int clientFd, const string &id)
    : fd_(clientFd),
      id_(id),
      recvLen_(0),
      needRecvLen_(HEADER_LEN),
      sendLen_(0),
      needSendLen_(0),
      recvStat_(RECVED),
      sendStat_(SENDED),
      recvBuf_(DEFAULT_BUFF_LEN)
{

}

bool Client::recvOnePck()
{
  if (getRecvStat_() == RECVED)
    return false;

  int recvLen = read(fd_, &recvBuf_[recvLen_], needRecvLen_);
  if (recvLen == 0)//对端关闭连接
  {
    setRecvStat_(TOCLOSE);
    LOG(ERROR) << "client:" << id_ << " closed!";
    return false;
  }
  if (recvLen == -1)
  {
    if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
    {
      return false; //需要继续接收数据
    }
    else
    {
      LOG(ERROR) << "unexpected recv error! need close this client:" << id_ << ", errno:" << errno;
      setRecvStat_(TOCLOSE);
      return false;
    }
  }
  if (recvLen < needRecvLen_)//还没收全一帧数据
  {
    recvLen_ += recvLen;
    needRecvLen_ -= recvLen;
    return false;
  }
  else
  {
    if (getRecvStat_() == RECVHEAD)  //收完头继续收payload
    {
      needRecvLen_ = *（(int *) &recvBuf_[0]);
      recvBuf_.resize(HEADER_LEN + needRecvLen_);
      setRecvStat_(RECVBODY);
      return false;
    }
    else if (getRecvStat_() == RECVBODY)
    {
      setRecvStat_(RECVED);
      return true;
    }
  }
}

bool Client::getPck(vector<char> &recvPck)
{
  if (getRecvStat_() == RECVED)
  {
    recvPck = recvBuf_;
    setRecvStat_(RECVHEAD);//取走数据继续接收
    recvLen_ = 0;
    return true;
  }
  return false;
}

bool Client::sendPck()
{
  if (getSendStat_() == SENDING)
  {
    int sendLen = write(fd_, &sendBuf_[sendLen_], sendBuf_.size() - sendLen_);
    if (sendLen == -1)
    {
      if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
      {
        return false; //需要继续发送数据
      }
      else
      {
        setSendStat_(TOCLOSE);
        LOG(ERROR) << "unexpected send error! need close this client:" << id_ << ", errno:" << errno;
      }
    }
    if (sendLen == 0)
    {
      setSendStat_(TOCLOSE);
      LOG(ERROR) << "client:" << id_ << " closed!";
      return false;
    }

    sendLen_ += sendLen;
    if (sendLen_ == sendBuf_.size())
    {
      setSendStat_(SENDED);
      return true;
    }
  }
  return false;
}

bool Client::setPck(const vector<char> &sendPck)
{
  if (getSendStat_() == SENDED)
  {
    int dataLen = sendPck.size();
    sendBuf_.resize(HEADER_LEN + dataLen);
    memcpy(&sendBuf_[0], &dataLen, HEADER_LEN);
    memcpy(&sendBuf_[0] + HEADER_LEN, &sendPck[0], dataLen);
    setSendStat_(SENDING);
    return true;
  }
  return false;
}

int Client::getFd_() const
{
  return fd_;
}

void Client::setFd_(int fd_)
{
  Client::fd_ = fd_;
}

int Client::getRecvStat_() const
{
  return recvStat_;
}

void Client::setRecvStat_(int stat_)
{
  Client::recvStat_ = stat_;
}
int Client::getSendStat_() const
{
  return sendStat_;
}
void Client::setSendStat_(int sendStat_)
{
  Client::sendStat_ = sendStat_;
}


