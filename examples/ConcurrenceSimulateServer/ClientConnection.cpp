//
// Created by Shinan on 2019/2/24.
//
#include "ClientConnection.h"
#include "ConcurrenceSimulateServer.h"

using namespace std::placeholders;

ClientConnection::ClientConnection(const SpStream &spStream,
                                   ConcurrenceSimulateServer &simulateServer)
    : Connection(spStream), simulateServer_(simulateServer)
{}

void ClientConnection::startReadOrWriteInService()
{
  //计算所有连接建立成功的时间
  if (simulateServer_.bConnectedFirstCount_)
  {
    simulateServer_.bConnectedFirstCount_ = false;
    simulateServer_.connectedCountStartTime_ = std::chrono::system_clock::now();
    LOG(ERROR) << "first client connected! fd:" << spStream_->getFd();

  }
  if (++simulateServer_.connectedCount_ == simulateServer_.clientNum_)
  {
    simulateServer_.connectedCountEndTime_ = std::chrono::system_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        simulateServer_.connectedCountEndTime_ - simulateServer_.connectedCountStartTime_).count();
    LOG(ERROR) << "all " << simulateServer_.connectedCount_ << " clients connected! duration:" << duration << " ms";
  }

  recvCompleteCallback_ = std::bind(&ClientConnection::recvCompleteCallback, this, _1, _2);
  sendCompleteCallback_ = std::bind(&ClientConnection::sendCompleteCallback, this, _1);
  //发送
  ss_.str("");
  ss_ << "client fd:" << spStream_->getFd() << " echo count : " << simulateServer_.echoCount_;
  int len = ss_.str().size();
  sendBuf_.resize(sizeof(len) + len);
  memcpy(&sendBuf_[0], &len, sizeof(len));
  memcpy(&sendBuf_[0 + sizeof(len)], ss_.str().c_str(), len);

  if (!spStream_->asyncSendBytes(sendBuf_, sendCompleteCallback_))
  {
    _destroy();
    return;
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
   // LOG(ERROR) << "recv error! client fd:" << spStream_->getFd() << " echoCount:" << simulateServer_.echoCount_;
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
    string str(vecBytes.begin(), vecBytes.end());

//    if (simulateServer_.echoCount_ % 1000 == 0)
//    {
//      LOG(INFO) << "1000 " << "echo msg received! msg : " << str;
//    }
    //   LOG(INFO) << "from Server, " << str;
    //收到全部信息则退出
    if (++simulateServer_.echoCount_ == simulateServer_.msgNum_ * simulateServer_.clientNum_)
    {
      simulateServer_.connectedRecvEndTime_ = std::chrono::system_clock::now();
      long long int duration = std::chrono::duration_cast<std::chrono::milliseconds>(
          simulateServer_.connectedRecvEndTime_ - simulateServer_.connectedSendStartTime_).count();
      LOG(INFO) << "all " << simulateServer_.echoCount_ <<" echo received! duration:" << duration << " ms";
      _destroy();
      exit(0);
    }
    //每个client收到自己全部的echo即断开连接
//    if (++selfEchoCount_ == simulateServer_.msgNum_)
//    {
//      _destroy();
//      return;
//    }
    stat_ = RECV_HEAD;
    spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_);
    return;
  }
}

void ClientConnection::sendCompleteCallback(int retSendStat)
{
  if (retSendStat == SENDERR)
  {
  //  LOG(ERROR) << "send error! client fd:" << getHandle() << " echoCount:" << simulateServer_.echoCount_;
    _destroy();
    return;
  }
  if (simulateServer_.bConnectedFirstSend_)
  {
    LOG(INFO) << "first client fd:" << spStream_->getFd() << " start to send";
    simulateServer_.bConnectedFirstSend_ = false;
    simulateServer_.connectedSendStartTime_ = std::chrono::system_clock::now();
  }
  ss_.str("");
  ss_ << "client fd:" << spStream_->getFd() << "echo count : " << simulateServer_.echoCount_;
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
  simulateServer_.connectionMap_.erase(getHandle());
}




