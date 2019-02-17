//
// Created by Shinan on 2019/2/14.
//

#include "NetWorkService.h"
#include "NewConnectMessage.h"

NetWorkService::NetWorkService(const string &name, const SpEventReactor &spEventReactor)
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
    SpStream spStream(new Stream(spNewConnectMsg->getClientFd(), spEventReactor_));
    if(newConnectionCb_){newConnectionCb_(spStream);}
  }
}

void NetWorkService::setNewConnectionCallback(const NetWorkService::ConnectionCallback &cb)
{
  newConnectionCb_ = cb;
}
