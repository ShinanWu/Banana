//
// Created by Shinan on 2019/1/23.
//
//Stream类封装了socket的io buffer，目前简单实现，每次最大1K数据，后面仍需考虑大数据小数据的情况
//非线程安全，不要尝试在其他线程操作此类实例

#include <unistd.h>
#include <errno.h>
#include <logging.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include "Stream.h"
#include "Message.h"

using namespace std::placeholders;

Stream::Stream(int fd, const SpEventReactor &spEventReactor)
    : fd_(fd),
      spEventReactor_(spEventReactor)
{
  //构造函数谨慎使用this
  recvOnePackCall_ = std::bind(&Stream::_recvOnePack, this, _1, _2);
  sendOnePackCall_ = std::bind(&Stream::_sendOnePack, this, _1, _2);
}

Stream::~Stream()
{
  if (fd_ > 0)
  {
    close(fd_);
    fd_ = -1;
  }
}

void Stream::_recvOnePack(int fd, short event)
{
  assert(fd_ == fd && event & EventReactor::EVENT_READ);
  int recvLen = recv(fd_, &recvBuf_[recvLen_], needRecvLen_, 0);
  //对端关闭连接
  if (recvLen == 0)
  {
    if (closeCallback_)
    { closeCallback_(fd_); }
    recvStat_ = RECVERR;
    LOG(ERROR) << "client:" << fd_ << " closed!";
    recvCompleteCallback_(recvStat_, recvBuf_);
    return;
  }
  if (recvLen < 0)
  {
    //此类错误需要继续接收数据
    if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
    {
      spEventReactor_->addEventHandler(fd_, EventReactor::EVENT_READ, recvOnePackCall_);
      recvStat_ = RECVING;
      return;
    }
    else
    {
      LOG(ERROR) << "unexpected recv error! shout down recv," << " errno:" << errno;
      spEventReactor_->removeEventHandler(fd_, EventReactor::EVENT_READ);
      if (closeCallback_)
      { closeCallback_(fd_); }
      //close fd，析构的时候再close,和stream生命周期保持一致
      recvStat_ = RECVERR;
      recvCompleteCallback_(recvStat_, recvBuf_);
      return;
    }
  }
  //还没收全数据
  if (recvLen < needRecvLen_)
  {
    recvLen_ += recvLen;
    needRecvLen_ -= recvLen;
    //需要继续接收数据
    spEventReactor_->addEventHandler(fd_, EventReactor::EVENT_READ, recvOnePackCall_);
    recvStat_ = RECVING;
    return;
  }
    //收到全部数据
  else
  {
    recvStat_ = RECVED;
    assert(recvCompleteCallback_);
    recvCompleteCallback_(recvStat_, recvBuf_);
  }
}

void Stream::_sendOnePack(int fd, short event)
{
  assert(fd_ == fd && event & EventReactor::EVENT_WRITE);

  int sendLen = send(fd_, &sendBuf_[sendLen_], needSendLen_, 0);
  if (sendLen < 0)
  {
    //继续发送
    if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
    {
      sendStat_ = SENDING;
      spEventReactor_->addEventHandler(fd_, EventReactor::EVENT_WRITE, sendOnePackCall_);
      return;
    }
    else
    {
      LOG(ERROR) << "unexpected send error! shout down send:" << fd_ << ", errno:" << errno;
      sendStat_ = SENDERR;
      sendCompleteCallback_(sendStat_);
      return;
    }
  }
  if (sendLen == 0)
  {
    LOG(ERROR) << "client:" << fd_ << " closed!";
    sendStat_ = SENDERR;
    sendCompleteCallback_(sendStat_);
    return;
  }
  //继续发送
  if (sendLen < needSendLen_)
  {
    sendLen_ += sendLen;
    needSendLen_ -= sendLen;
    sendStat_ = SENDING;
    spEventReactor_->addEventHandler(fd_, EventReactor::EVENT_WRITE, sendOnePackCall_);
    return;
  }
    //发送完毕
  else
  {
    sendStat_ = SENDED;
    sendCompleteCallback_(sendStat_);
  }
}

//bool Stream::setPck(const string &sendPck)
//{
//  if (getSendStat_() == SENDED)
//  {
//    int dataLen = sendPck.size();
//    sendBuf_.resize(HEADER_LEN + dataLen);
//    memcpy(&sendBuf_[0], &dataLen, HEADER_LEN);
//    memcpy((char *) (&sendBuf_[0]) + HEADER_LEN, sendPck.data(), dataLen);
//    return true;
//  }
//  return false;
//}

int Stream::getFd_() const
{
  return fd_;
}

int Stream::getRecvStat_() const
{
  return recvStat_;
}

int Stream::getSendStat_() const
{
  return sendStat_;
}

void Stream::setOnCloseCallback_(SocketCloseCallback &closeCallback)
{
  closeCallback_ = std::move(closeCallback); //一般只设一次
}

void Stream::asyncRecvBytes(int num, RecvCompleteCallback &recvCompleteCallback)
{
  if (!recvCompleteCallback || num > DEFAULT_BUFF_LEN || num <= 0)
  {
    LOG(ERROR) << "parameter error!";
    return;
  }
  recvLen_ = 0;
  needRecvLen_ = num;
  recvCompleteCallback_ = recvCompleteCallback;
  spEventReactor_->addEventHandler(fd_, EventReactor::EVENT_READ, recvOnePackCall_);
}

void Stream::asyncSendBytes(const vector<char> &vecBytes, Stream::SendCompleteCallback &sendCompleteCallback)
{
  int sendSize = vecBytes.size();
  if (!sendCompleteCallback || sendSize > DEFAULT_BUFF_LEN)
  {
    LOG(ERROR) << "parameter error!";
    return;
  }
  sendLen_ = 0;
  needSendLen_ = sendSize;
  sendBuf_ = vecBytes;
  sendCompleteCallback_ = sendCompleteCallback;
  spEventReactor_->addEventHandler(fd_, EventReactor::EVENT_WRITE, sendOnePackCall_);
}

const SpEventReactor &Stream::getSpEventReactor_() const
{
  return spEventReactor_;
}


