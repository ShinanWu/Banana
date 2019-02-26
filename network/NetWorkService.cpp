//
// Created by Shinan on 2019/2/14.
//
#include <assert.h>
#include <utils/Utils.h>
#include "NetWorkService.h"
#include "NewConnectMessage.h"
#include "ConnectionMessage.h"
#include "Connection.h"

using namespace Utils;
using namespace std::placeholders;

NetWorkService::NetWorkService(const string &name, const SpEventReactor &spEventReactor)
    : InteractiveTask(name, spEventReactor)
{
  onNewConMsgCall_ = std::bind(&NetWorkService::onNewConMsg, this, _1, _2);
}

NetWorkService::~NetWorkService()
{}

void NetWorkService::onStart()
{
  acceptEventFd_ = Utils::createEventFd();
  assert(acceptEventFd_);
  spEventReactor_->addEventHandler(acceptEventFd_, EventReactor::EVENT_READ, onNewConMsgCall_);
}

void NetWorkService::onStop()
{}

void NetWorkService::onMessage(const shared_ptr<Message> &spMessage)
{
  if (spMessage->getWhat() == "NEW_CONNECT")
  {
    auto spNewConnectMsg = dynamic_pointer_cast<NewConnectMessage>(spMessage);
    SpStream spStream(new Stream(spNewConnectMsg->getClientFd(), spEventReactor_));
    if (newConnectionCb_)
    { newConnectionCb_(spStream, dynamic_pointer_cast<NetWorkService>(shared_from_this())); }
    return;
  }

  if (spMessage->getWhat() == "CONNECTION")
  {
    auto spConnectionMsg = dynamic_pointer_cast<ConnectionMessage>(spMessage);
    auto it = connectionMap_.find(spConnectionMsg->getHandle());
    if (it != connectionMap_.end())
    {
      it->second->onMessage(spConnectionMsg);
    }
    else
    {
      LOG(ERROR) << "can't find client, handle: " << spConnectionMsg->getHandle();
    }
  }
  else
  { assert(0); }

}

void NetWorkService::setNewConnectionCallback(const ConnectionCallback &cb)
{
  newConnectionCb_ = cb;
}

bool NetWorkService::addNewConnection(const SpConnection &spConnection)
{
  auto retPair = connectionMap_.insert(std::make_pair(spConnection->getHandle(), spConnection));
  return retPair.second;
}

void NetWorkService::removeConnection(int handle)
{
  connectionMap_.erase(handle);
}

bool NetWorkService::notifyNewConnnectMsg(const NewConnectMessage &newConMsg)
{
  if (!newConMsgBuf_.asyncPut(newConMsg))
  {
    LOG(ERROR) << "service " << getTaskName() << " is busy!";
    return false;
  }
  unsigned long long eventVal = 1;
  write(acceptEventFd_, &eventVal, sizeof(eventVal)); //内部counter会加1
  return true;

}

void NetWorkService::onNewConMsg(int fd, short event)
{
  assert(acceptEventFd_ == fd);

  unsigned long long eventVal;
  read(fd, &eventVal, sizeof(eventVal)); //内部counter会减1
  assert(eventVal == 1);

  NewConnectMessage newConMsg;
  assert(newConMsgBuf_.asyncGet(newConMsg));

  SpStream spStream(new Stream(newConMsg.getClientFd(), spEventReactor_));

  if (newConnectionCb_)
  { newConnectionCb_(spStream, dynamic_pointer_cast<NetWorkService>(shared_from_this())); }

  spEventReactor_->addEventHandler(acceptEventFd_, EventReactor::EVENT_READ, onNewConMsgCall_);
  return;
}
