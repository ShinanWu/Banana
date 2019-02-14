//
// Created by Shinan on 2019/2/14.
//

#include "NetWorkerService.h"

NetWorkerService::NetWorkerService(const string &name, const shared_ptr<EventReactor> &spEventReactor)
    : InteractiveTask(name, spEventReactor)
{}

NetWorkerService::~NetWorkerService()
{

}

bool NetWorkerService::_onStart()
{
  return false;
}

void NetWorkerService::_onStop()
{

}

void NetWorkerService::_onMessage(const shared_ptr<Message> &spMessage)
{

}
