//
// Created by Shinan on 2019/1/22.
//

#ifndef SEVICEFRAMEWORK_NETSERVICE_H
#define SEVICEFRAMEWORK_NETSERVICE_H

#include <InteractiveTask.h>

class NetService : public InteractiveTask
{
public:
  NetService(const string &name);
  virtual ~NetService();

protected:
  bool _onStart();
  void _onStop();
  void _onMessage(const shared_ptr<Message> &spMessage);

private:
  void __onAccept(int fd);
};

#endif //SEVICEFRAMEWORK_NETSERVICE_H
