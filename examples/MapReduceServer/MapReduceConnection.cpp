//
// Created by Shinan on 2019/2/18.
//
#include <fstream>
#include "utils/Utils.h"
#include "MapReduceConnection.h"
#include "MapDoneConnectionMessage.h"
#include "MapTask.h"
#include "multi-threading/ThreadPool.h"
using namespace Utils;
using namespace std::placeholders;

MapReduceConnection::MapReduceConnection(const SpStream &spStream, const SpNetWorkService &spNetWorkService)
    : Connection(spStream, spNetWorkService)
{}

MapReduceConnection::~MapReduceConnection()
{}

void MapReduceConnection::startReadOrWriteInService()
{
  stat_ = RECV_HEAD;
  recvCompleteCallback_ = std::bind(&MapReduceConnection::recvCompleteCallback, shared_from_this(), _1, _2);
  sendCompleteCallback_ = std::bind(&MapReduceConnection::sendCompleteCallback, shared_from_this(), _1);
  spStream_->asyncRecvBytes(sizeof(size_t), recvCompleteCallback_);
}

void MapReduceConnection::onMessage(const SpConnectionMessage &spConnectionMessage)
{
  auto spMapDoneConnectionMessage = dynamic_pointer_cast<MapDoneConnectionMessage>(spConnectionMessage);
  taskCount_--;
  if (firstCome_)
  {
    curMaxNum_ = spMapDoneConnectionMessage->getMaxNum_();
    firstCome_ = false;
  }
  else
  {
    curMaxNum_ = reduce(curMaxNum_, spMapDoneConnectionMessage->getMaxNum_());
  }
  if (taskCount_ == 0) //reduce阶段结束
  {
    vector<char> vecChar;
    char *p = (char *)(&curMaxNum_);
    for (int i = 0; i < sizeof(curMaxNum_); i ++)
    {
      vecChar.emplace_back(p[i]);
    }
    spStream_->asyncSendBytes(vecChar, sendCompleteCallback_);
  }

}

void MapReduceConnection::recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes)
{
  if (stat_ == RECV_HEAD)
  {
    if(vecBytes.size() < sizeof(size_t))
    {
      LOG(ERROR) << "message decode error, size is wrong! discard it!";
      spStream_->asyncRecvBytes(sizeof(size_t), recvCompleteCallback_);
      return;
    }
    size_t bodyLen = *(size_t *)(&vecBytes[0]);
    spStream_->asyncRecvBytes(bodyLen, recvCompleteCallback_);
    stat_ = RECV_BODY;
    return;
  }
  if (stat_ == RECV_BODY)
  {
    string str(vecBytes.begin(), vecBytes.end());
    vector<string> vecStr;
    split(str, vecStr, ' ');
    for (auto& path : vecStr) //map阶段开始
    {
      auto spTask = make_shared<MapTask>(bind(&MapReduceConnection::mapFunction, this, path));
      ThreadPool::getInstance()->syncPostTask(spTask);
      taskCount_ ++;
    }
  }

}

void MapReduceConnection::sendCompleteCallback(int retSendStat)
{
  LOG(INFO) << "map-reduce task done!";
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
  int maxNum;
  int tmp;
  ifstream file(filePath);
  assert(file.is_open());
  file >> maxNum;
  while (file >> tmp)
  {
    if (tmp > maxNum) maxNum = tmp;
  }
  auto spMsg = make_shared<MapDoneConnectionMessage>(getHandle(), maxNum);
  spNetWorkService_->notifyMsg(spMsg);
}

int MapReduceConnection::reduce(int last, int cur)
{
  return cur > last ? cur : last;
}

