//
// Created by Shinan on 2019/2/14.
//

#ifndef SERVICEFRAMEWORK_NETWORKERSERVICE_H
#define SERVICEFRAMEWORK_NETWORKERSERVICE_H

#include <InteractiveTask.h>
class NetWorkerService : public InteractiveTask
{
public:
  NetWorkerService(const string &name, const shared_ptr<EventReactor> &spEventReactor);
  virtual ~NetWorkerService();
private:
  virtual void onStart();
  virtual void onStop();
  virtual void onMessage(const shared_ptr<Message> &spMessage);

};

#endif //SERVICEFRAMEWORK_NETWORKERSERVICE_H
