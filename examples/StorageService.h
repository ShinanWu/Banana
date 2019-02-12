//
// Created by Shinan on 2019/2/11.
//

#ifndef SEVICEFRAMEWORK_STORAGESERVICE_H
#define SEVICEFRAMEWORK_STORAGESERVICE_H

#include "InteractiveTask.h"

class StorageService : public InteractiveTask
{
public:
  StorageService();
  StorageService(const string &name);
  virtual ~StorageService();
protected:
  virtual bool _onStart();
  virtual void _onStop();
  virtual void _onMessage(const shared_ptr<Message> &spMessage);
};

#endif //SEVICEFRAMEWORK_STORAGESERVICE_H
