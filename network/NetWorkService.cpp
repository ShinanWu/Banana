//
// Created by Shinan on 2019/2/14.
//

#include "NetWorkService.h"
#include "NewConnectMessage.h"

NetWorkService::NetWorkService(const string &name, const shared_ptr<EventReactor> &spEventReactor)
    : InteractiveTask(name, spEventReactor)
{}

NetWorkService::~NetWorkService()
{}

void NetWorkService::onStart()
{

}

void NetWorkService::onStop()
{

}

void NetWorkService::onMessage(const shared_ptr<Message> &spMessage)
{
  if (spMessage->getWhat() == "NEW_CONNECT")
  {
    auto spNewConnectMsg = dynamic_pointer_cast<NewConnectMessage>(spMessage);
    shared_ptr<Stream> spStream(new Stream(spNewConnectMsg->getClientFd()));
    if(newConnectionCb_){newConnectionCb_(spStream);}
  }
}

void NetWorkService::setNewConnectionCallback(NetWorkService::ConnectionCallback &cb)
{
  newConnectionCb_ = std::move(cb);
}
