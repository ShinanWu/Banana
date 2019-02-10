//
// Created by Shinan on 2019/1/18.
//
//可以相互通信的task的基类，子类只需要实现onStart,onStop,onMessage等函数，
//生命周期也即如此函数所述
//
#ifndef SERVICEFRAMEWORK_INTERACTIVETASK_H
#define SERVICEFRAMEWORK_INTERACTIVETASK_H

#include <string>
#include <memory>
#include <unordered_map>
#include <logging.h>
#include "Task.h"
#include "ConcurrentRingBuffer.h"
#include "Message.h"
#include "network/EventReactor.h"

using namespace std;

class InteractiveTask : public Task
{
  typedef ConcurrentRingBuffer<shared_ptr<Message>> MsgQueue;

public:
  InteractiveTask(const string &name, std::shared_ptr<EventReactor> eventReactor = nullptr);
  virtual ~InteractiveTask();
  void start();
  void stop();
  virtual bool onStart() = 0;
  virtual bool onStop() = 0;
  virtual void onMessage(const shared_ptr<Message> &spMessage) = 0; //接收Task间消息的处理函数
  int sendMsg(const string &taskName, const shared_ptr<Message> &spMessage);
  int notifyMsg(const shared_ptr<Message> &spMessage);

private:
  void _onMessage(int fd, short event);
  bool _setThreadName(const string &name);
  int _createEventFd();

private:
  std::shared_ptr<MsgQueue> recvMsgQueue_;//每个InteractiveTask独占一个消息队列，生命周期和task一致
  std::shared_ptr<EventReactor> eventReactor_;
  int eventFd_;
};

#endif //SERVICEFRAMEWORK_INTERACTIVETASK_H
