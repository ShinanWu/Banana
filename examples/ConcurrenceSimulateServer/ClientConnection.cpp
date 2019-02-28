//
// Created by Shinan on 2019/2/24.
//
#include "ClientConnection.h"
#include "ConcurrenceSimulateServer.h"

using namespace std::placeholders;

ClientConnection::ClientConnection(const SpStream &spStream,
                                   const WpNetWorkService &wpNetWorkService,
                                   ConcurrenceSimulateServer &simulateServer)
    : Connection(spStream, wpNetWorkService),
      simulateServer_(simulateServer)
    // clientNum_(simulateServer.clientNum_ / 2),
    //  msgNum_(simulateServer.msgNum_)
{}

void ClientConnection::startReadOrWriteInService()
{

  recvCompleteCallback_ = std::bind(&ClientConnection::recvCompleteCallback, this, _1, _2);
  sendCompleteCallback_ = std::bind(&ClientConnection::sendCompleteCallback, this, _1);
  //发送
  ss_.str("");
  ss_ << "client fd:" << spStream_->getFd() << " echo hello!";
  int len = ss_.str().size();
  sendBuf_.resize(sizeof(len) + len);
  memcpy(&sendBuf_[0], &len, sizeof(len));
  memcpy(&sendBuf_[0 + sizeof(len)], ss_.str().c_str(), len);

  if (!spStream_->asyncSendBytes(sendBuf_, sendCompleteCallback_))
  {
    _destroy();
    return;
  }
  //非线程安全
  if (simulateServer_.bConnectedFirstSend_)
  {
    simulateServer_.bConnectedFirstSend_ = false;
    simulateServer_.connectedSendStartTime_ = std::chrono::system_clock::now();
  }

  //接收
  stat_ = RECV_HEAD;
  if (!spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_))
  {
    _destroy();
    return;
  }

}

void ClientConnection::recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes)
{
  if (retRecvStat == RECVERR)
  {
     LOG(ERROR) << "recv error! client fd:" << spStream_->getFd() << " echoCount:" << selfEchoCount_;
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
      LOG(ERROR) << getSpNetWorkService()->getTaskName() << "message decode error, body size is too long! discard it!";
      spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_);
      return;
    }
    spStream_->asyncRecvBytes(bodyLen, recvCompleteCallback_);
    stat_ = RECV_BODY;
    return;
  }
  if (stat_ == RECV_BODY)
  {
    string str(vecBytes.begin(), vecBytes.end());
    //收到全部信息则退出
    if (++simulateServer_.echoCount_ >= simulateServer_.msgNum_ * simulateServer_.clientNum_)
    {
      simulateServer_.connectedRecvEndTime_ = std::chrono::system_clock::now();
      long long int duration = std::chrono::duration_cast<std::chrono::milliseconds>(
          simulateServer_.connectedRecvEndTime_ - simulateServer_.connectedSendStartTime_).count();
      LOG(INFO) << "all " << simulateServer_.echoCount_ << " echo received! duration:"
                << duration << " ms";
      google::FlushLogFiles(google::INFO);
      _destroy();
      exit(0);
    }
    if (simulateServer_.echoCount_ % 10000 == 0)
    {
      LOG(INFO) << "10000 " << "echo msg received! msg : " << str;
      google::FlushLogFiles(google::INFO);
    }
    //每个client收到自己全部的echo即停止发送，但不释放连接
    if (++selfEchoCount_ == simulateServer_.msgNum_)
    {
 //     _destroy();
      return;
    }
    stat_ = RECV_HEAD;
    spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_);
    return;
  }
}

void ClientConnection::sendCompleteCallback(int retSendStat)
{
  if (retSendStat == SENDERR)
  {
      LOG(ERROR) << "send error! client fd:" << getHandle() << " echoCount:" << selfEchoCount_;
    _destroy();
    return;
  }
  if (selfEchoCount_ == simulateServer_.msgNum_)
    return;
  ss_.str("");
  ss_ << "client fd:" << spStream_->getFd() << " echo hello!";
  int len = ss_.str().size();
  sendBuf_.resize(sizeof(len) + len);
  memcpy(&sendBuf_[0], &len, sizeof(len));
  memcpy(&sendBuf_[0 + sizeof(len)], ss_.str().c_str(), len);
  if (!spStream_->asyncSendBytes(sendBuf_, sendCompleteCallback_))
  {
    _destroy();
    return;
  }
}

void ClientConnection::_destroy()
{
  //spStream_->destory();
  //spStream_ = nullptr;
  // recvCompleteCallback_ = nullptr;
  //sendCompleteCallback_ = nullptr;
  //Connection::destroy();
  auto spNetWorkService = wpNetWorkService_.lock();
  spNetWorkService->removeConnection(getHandle());
}




