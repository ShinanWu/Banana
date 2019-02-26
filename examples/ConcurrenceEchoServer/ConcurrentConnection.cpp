//
// Created by Shinan on 2019/2/24.
//

#include "ConcurrentConnection.h"
using namespace std::placeholders;

ConcurrentConnection::ConcurrentConnection(const SpStream &spStream) : Connection(spStream)
{}

ConcurrentConnection::ConcurrentConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService)
    : Connection(spStream, wpNetWorkService)
{}

void ConcurrentConnection::startReadOrWriteInService()
{
  recvCompleteCallback_ = std::bind(&ConcurrentConnection::recvCompleteCallback, this, _1, _2);
  sendCompleteCallback_ = std::bind(&ConcurrentConnection::sendCompleteCallback, this, _1);

  stat_ = RECV_HEAD;
  if (!spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_))
  {
    _destroy();
    return;
  }
}

void ConcurrentConnection::recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes)
{
  if (retRecvStat == RECVERR)
  {
   // LOG(ERROR) << "recv error! wait to lose this client, fd " << spStream_->getFd();
    _destroy();
    return;
  }
  if (stat_ == RECV_HEAD)
  {
    if (vecBytes.size() < sizeof(int))
    {
      LOG(ERROR) << "message decode error, head size is wrong! discard it!";
      spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_);
      return;
    }
    int bodyLen = *(int *) (&vecBytes[0]);
    if (bodyLen > 1024)
    {
      LOG(ERROR) << "message decode error, body size is too long! discard it!";
      spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_);
      return;
    }
    spStream_->asyncRecvBytes(bodyLen, recvCompleteCallback_);
    stat_ = RECV_BODY;
    return;
  }
  if (stat_ == RECV_BODY)
  {
    //接收完毕开始发送
    string str(vecBytes.begin(), vecBytes.end());
    int len = str.size();
    recvBuf_.resize(sizeof(len) + len);
    memcpy(&recvBuf_[0], &len, sizeof(len));
    memcpy(&recvBuf_[0 + sizeof(len)], str.c_str(), len);

    spStream_->asyncSendBytes(recvBuf_, sendCompleteCallback_);
    return;
  }
}

void ConcurrentConnection::sendCompleteCallback(int retSendStat)
{
  if (retSendStat == SENDERR)
  {
   // LOG(ERROR) << "send error! data: " << string(recvBuf_.begin(), recvBuf_.end());
    _destroy();
    return;
  }
  //发送完毕开始接收
  stat_ = RECV_HEAD;
  if (!spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_))
  {
    _destroy();
    return;
  }
}

void ConcurrentConnection::_destroy()
{
//  spStream_->destory();
  //spStream_.reset();
  //释放自己的引用，防止内存泄漏
  //recvCompleteCallback_ = nullptr;
  //sendCompleteCallback_ = nullptr;
  //释放context空间，即销毁此Connection,最后关闭fd可以防止新来的连接复用此fd导致错误的继续使用此context空间
  Connection::destroy();
}

