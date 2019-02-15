//
// Created by Shinan on 2019/1/22.
//

#ifndef SEVICEFRAMEWORK_NETSERVICE_H
#define SEVICEFRAMEWORK_NETSERVICE_H

#include <InteractiveTask.h>

class NetAcceptService : public InteractiveTask
{
public:
  NetAcceptService(const string &name, const shared_ptr<EventReactor> &spEventReactor);
  virtual ~NetAcceptService();

private:
  void onStart();
  void onStop();
  void onMessage(const shared_ptr<Message> &spMessage);
  void __onAccept(int fd);
};

#endif //SEVICEFRAMEWORK_NETSERVICE_H
