//
// Created by Shinan on 2019/1/22.
//

#ifndef SEVICEFRAMEWORK_NETSERVICE_H
#define SEVICEFRAMEWORK_NETSERVICE_H

#include <InteractiveTask.h>
#include "NetWorkService.h"

class NetAcceptService : public InteractiveTask
{
public:
  NetAcceptService(const string &name, const shared_ptr<EventReactor> &spEventReactor);
  virtual ~NetAcceptService();
  void addNetWorkService(const shared_ptr<NetWorkService>& spWorkService);

private:
  void onStart() final ;
  void onStop() final ;
  void onMessage(const shared_ptr<Message> &spMessage) final ;
  void __onAccept(int fd);

private:
  vector<shared_ptr<NetWorkService>> vecSpWorkService_;
  int curServiceIndex_ = 0;
  int _nextServiceIndex();
};

#endif //SEVICEFRAMEWORK_NETSERVICE_H
