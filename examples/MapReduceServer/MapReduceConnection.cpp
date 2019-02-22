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

MapReduceConnection::MapReduceConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService)
    : Connection(spStream, wpNetWorkService)
{}

MapReduceConnection::~MapReduceConnection()
{
  LOG(ERROR) << "MapReduceConnection destructed!";
}

void MapReduceConnection::startReadOrWriteInService()
{
  recvCompleteCallback_ = std::bind(&MapReduceConnection::recvCompleteCallback, this, _1, _2);
  sendCompleteCallback_ = std::bind(&MapReduceConnection::sendCompleteCallback, this, _1);

  stat_ = RECV_HEAD;
  if (!spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_))
  {
    _destroy();
  }
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
    char *p = (char *) (&curMaxNum_);
    for (int i = 0; i < sizeof(curMaxNum_); i++)
    {
      vecChar.emplace_back(p[i]);
    }
    spStream_->asyncSendBytes(vecChar, sendCompleteCallback_);
  }

}

void MapReduceConnection::recvCompleteCallback(int retRecvStat, const vector<char> &vecBytes)
{
  if (retRecvStat == RECVERR)
  {
    LOG(ERROR) << "recv error! wait to lose this client, fd " << spStream_->getFd();
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
    if(bodyLen > 1024)
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
    vector<string> vecStr;
    split(str, vecStr, ' ');
    for (auto &path : vecStr) //map阶段开始
    {
      auto spTask = make_shared<MapTask>(bind(MapReduceConnection::mapFunction, shared_from_this(), path));
      ThreadPool::getInstance()->syncPostTask(spTask);
      taskCount_++;
    }
    stat_ = RECV_HEAD;
    spStream_->asyncRecvBytes(sizeof(int), recvCompleteCallback_);
    return;
  }
}

void MapReduceConnection::sendCompleteCallback(int retSendStat)
{
  LOG(INFO) << "map-reduce task done!";
  if (retSendStat == SENDERR)
  {
    LOG(ERROR) << "send error! wait to lose this client, fd " << spStream_->getFd();
    _destroy();
  }
}

void MapReduceConnection::_destroy()
{
  //释放自己的引用，防止内存泄漏
  recvCompleteCallback_ = nullptr;
  sendCompleteCallback_ = nullptr;
  //释放context空间，即销毁此Connection,最后关闭fd可以防止新来的连接复用此fd导致错误的继续使用此context空间
  Connection::destroy();
}

//注意这是static函数，可以观察发起者的生命周期，参数只能传值，不要传引用，因为在其他线程执行，类似可重入函数
void MapReduceConnection::mapFunction(weak_ptr<MapReduceConnection> wpConnection, const string filePath)
{
  auto spConnection = wpConnection.lock();
  //如果已析构就结束
  if (!spConnection) return;
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
  auto spMsg = make_shared<MapDoneConnectionMessage>(spConnection->getHandle(), maxNum);
  auto spService = spConnection->getSpNetWorkService();
  //回复发起者
  spService->notifyMsg(spMsg);

}

int MapReduceConnection::reduce(int last, int cur)
{
  return cur > last ? cur : last;
}

