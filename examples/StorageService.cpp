//
// Created by Shinan on 2019/2/11.
//
#include "logging.h"
#include "StorageService.h"

StorageService::StorageService(const string &name) : InteractiveTask(name)
{}

StorageService::~StorageService()
{}

bool StorageService::_onStart()
{
  LOG(INFO) << "StorageService " << getTaskName() << " stared!";
  return true;
}

void StorageService::_onStop()
{
  LOG(INFO) << "StorageService " << getTaskName() << " stopped!";
}

void StorageService::_onMessage(const shared_ptr<Message> &spMessage)
{
  LOG(INFO) << "message " << spMessage->getWhat() << " received!";
}

