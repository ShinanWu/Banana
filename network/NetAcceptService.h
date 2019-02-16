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
  void onStart() final ;
  void onStop() final ;
  void onMessage(const shared_ptr<Message> &spMessage) final ;
  void __onAccept(int fd);
};

#endif //SEVICEFRAMEWORK_NETSERVICE_H
