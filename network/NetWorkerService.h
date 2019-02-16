//
// Created by Shinan on 2019/2/14.
//

#ifndef SERVICEFRAMEWORK_NETWORKERSERVICE_H
#define SERVICEFRAMEWORK_NETWORKERSERVICE_H

#include <InteractiveTask.h>
#include "Stream.h"
class NetWorkerService : public InteractiveTask
{
public:
  NetWorkerService(const string &name, const shared_ptr<EventReactor> &spEventReactor);
  virtual ~NetWorkerService();
private:
  virtual void onStart() final;
  virtual void onStop() final;
  virtual void onMessage(const shared_ptr<Message> &spMessage) final;


};

#endif //SERVICEFRAMEWORK_NETWORKERSERVICE_H
