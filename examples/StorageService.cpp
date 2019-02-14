//
// Created by Shinan on 2019/2/11.
//
#include <unistd.h>
#include <assert.h>
#include "logging.h"
#include "StorageService.h"
#include "StringMessage.h"

StorageService::StorageService(const string &name) : InteractiveTask(name)
{}

StorageService::~StorageService()
{}

bool StorageService::_onStart()
{
 // LOG(INFO) << "Task " << getTaskName() << " stared!";
  if(getTaskName() == "Service-2")
  {
  //  sleep(3);
 //   LOG(INFO) << "send msg to StorageService-2";
    _sendMsgTo("Service-1", shared_ptr<Message>(new StringMessage(getTaskName(), nullptr)));
  }
  if(getTaskName() == "Service-9")
  {
   // sleep(3);
    //   LOG(INFO) << "send msg to StorageService-2";
    _sendMsgTo("Service-1", shared_ptr<Message>(new StringMessage(getTaskName(), nullptr)));
  }
  if(getTaskName() == "Service-20")
  {
  //  sleep(3);
    //   LOG(INFO) << "send msg to StorageService-2";
    _sendMsgTo("Service-2", shared_ptr<Message>(new StringMessage(getTaskName(), nullptr)));
  }
  if(getTaskName() == "Service-39")
  {
  //  sleep(3);
    //   LOG(INFO) << "send msg to StorageService-2";
    _sendMsgTo("Service-2", shared_ptr<Message>(new StringMessage(getTaskName(), nullptr)));
  }
  return true;
}

void StorageService::_onStop()
{
  LOG(INFO) << "Task " << getTaskName() << " stopped!";
}

void StorageService::_onMessage(const shared_ptr<Message> &spMessage)
{
  LOG(INFO) << getTaskName() << "recv message from" << spMessage->getWhat();
}

