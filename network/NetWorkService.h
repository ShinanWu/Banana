//
// Created by Shinan on 2019/2/14.
//

#ifndef SERVICEFRAMEWORK_NETWORKERSERVICE_H
#define SERVICEFRAMEWORK_NETWORKERSERVICE_H

#include <functional>
#include <InteractiveTask.h>
#include "Stream.h"
using namespace std;

class NetWorkService : public InteractiveTask
{
  typedef function<void(const SpStream& spStream)> ConnectionCallback;
public:
  NetWorkService(const string &name, const SpEventReactor &spEventReactor);
  virtual ~NetWorkService();
  void setNewConnectionCallback(const ConnectionCallback &cb);

private:
  virtual void onStart() final;
  virtual void onStop() final;
  virtual void onMessage(const shared_ptr<Message> &spMessage) final;

private:
  ConnectionCallback newConnectionCb_;
};

#endif //SERVICEFRAMEWORK_NETWORKERSERVICE_H
