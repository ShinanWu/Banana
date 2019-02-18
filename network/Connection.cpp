//
// Created by Shinan on 2019/1/27.
//

#include "Connection.h"
#include "Stream.h"

Connection::Connection(const SpStream &spStream, const SpNetWorkService &spNetWorkService)
    : spStream_(spStream), spNetWorkService_(spNetWorkService)
{
  handle_ = spStream->getFd();
  assert(handle_ > 0);
}

Connection::~Connection()
{}

int Connection::getHandle() const
{
  return handle_;
}

void Connection::destroy()
{
  spNetWorkService_->removeConnection(handle_);
}

void Connection::startReadOrWriteInService()
{

}

void Connection::onMessage(const SpConnectionMessage &spConnectionMessage)
{

}

