//
// Created by Shinan on 2019/2/14.
//
#include <assert.h>
#include "NetWorkService.h"
#include "NewConnectMessage.h"
#include "ConnectionMessage.h"


NetWorkService::NetWorkService(const string &name, const SpEventReactor &spEventReactor)
    : InteractiveTask(name, spEventReactor)
{}

NetWorkService::~NetWorkService()
{}

void NetWorkService::onStart()
{}

void NetWorkService::onStop()
{}

void NetWorkService::onMessage(const shared_ptr<Message> &spMessage)
{
  if (spMessage->getWhat() == "NEW_CONNECT")
  {
    auto spNewConnectMsg = dynamic_pointer_cast<NewConnectMessage>(spMessage);
    SpStream spStream(new Stream(spNewConnectMsg->getClientFd(), spEventReactor_));
    if (newConnectionCb_)
    { newConnectionCb_(spStream,  dynamic_pointer_cast<NetWorkService>(shared_from_this())); }
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
      assert(0);
    }
  }
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
