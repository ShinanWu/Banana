//
// Created by Shinan on 2019/1/23.
//
//每一个连接的客户端生成一个client对象,封装了接收和发送消息
//非线程安全

#include <unistd.h>
#include "Stream.h"
#include "Message.h"
#include <errno.h>
#include <logging.h>
#include <string.h>

Stream::Stream(int fd)
    : fd_(fd),
      recvLen_(0),
      needRecvLen_(HEADER_LEN),
      sendLen_(0),
      needSendLen_(0),
      recvStat_(RECVED),
      sendStat_(SENDED),
      recvBuf_(DEFAULT_BUFF_LEN)
{

}

bool Stream::recvOnePck()
{
  if (getRecvStat_() == RECVED)
    return true;

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

bool Stream::getPck(vector<char> &recvPck)
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

bool Stream::sendPck()
{
  if(getSendStat_() == SENDED)
  {
    return true;
  }
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

bool Stream::setPck(const vector<char> &sendPck)
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

int Stream::getFd_() const
{
  return fd_;
}

void Stream::setFd_(int fd_)
{
  Stream::fd_ = fd_;
}

int Stream::getRecvStat_() const
{
  return recvStat_;
}

void Stream::setRecvStat_(int stat_)
{
  Stream::recvStat_ = stat_;
}
int Stream::getSendStat_() const
{
  return sendStat_;
}
void Stream::setSendStat_(int sendStat_)
{
  Stream::sendStat_ = sendStat_;
}


