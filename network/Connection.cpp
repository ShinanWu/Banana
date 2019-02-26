//
// Created by Shinan on 2019/1/27.
//每一个连接的操作空间，不区分主动连接还是被动连接
//新连接生成需要添加到所属loop的map中，管理生命周期和端对端的通信
//销毁需要调用基类destory函数从map中移除。

#include "Connection.h"
#include "NetWorkService.h"
#include "Stream.h"

Connection::Connection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService)
    : spStream_(spStream), wpNetWorkService_(wpNetWorkService)
{
  handle_ = spStream->getFd();
  assert(handle_ > 0);
}

Connection::Connection(const SpStream &spStream)
    : spStream_(spStream)
{
  handle_ = spStream->getFd();
  assert(handle_ > 0);
}

Connection::~Connection()
{
 // LOG(ERROR) << "~Connection";
}

int Connection::getHandle() const
{
  return handle_;
}

SpNetWorkService Connection::getSpNetWorkService()
{
  return wpNetWorkService_.lock();
}

void Connection::destroy()
{
  auto sp = wpNetWorkService_.lock();
  //消息队列可能仍有引用，销毁不及时
  sp->removeConnection(handle_);
}

void Connection::startReadOrWriteInService()
{

}

void Connection::onMessage(const SpConnectionMessage &spConnectionMessage)
{

}


