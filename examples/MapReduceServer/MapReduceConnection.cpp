//
// Created by Shinan on 2019/2/18.
//

#include "MapReduceConnection.h"
#include "MapDoneConnectionMessage.h"

using namespace std::placeholders;

MapReduceConnection::MapReduceConnection(const SpStream &spStream, const SpNetWorkService &spNetWorkService)
    : Connection(spStream, spNetWorkService)
{}

MapReduceConnection::~MapReduceConnection()
{}

void MapReduceConnection::startReadOrWriteInService()
{
  recvCompleteCallback_ = std::bind(&MapReduceConnection::recvCompleteCallback, shared_from_this(), _1, _2);
  sendCompleteCallback_ = std::bind(&MapReduceConnection::sendCompleteCallback, shared_from_this(), _1);
  spStream_->asyncRecvBytes(4, recvCompleteCallback_);
}

void MapReduceConnection::onMessage(const SpConnectionMessage &spConnectionMessage)
{

}

void MapReduceConnection::recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes)
{
   string(vecBytes.begin(), vecBytes.end());
}

void MapReduceConnection::sendCompleteCallback(int retSendStat)
{

}

void MapReduceConnection::_destroy()
{
  recvCompleteCallback_ = nullptr; //释放自己的引用，防止内存泄漏
  sendCompleteCallback_ = nullptr;
  Connection::destroy();
}

void MapReduceConnection::mapFunction(const string &filePath)
{
  //do disk IO
  auto spMsg = make_shared<MapDoneConnectionMessage>(getHandle(), )
  spNetWorkService_->notifyMsg(spMsg);
}
