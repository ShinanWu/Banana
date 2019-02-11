//
// Created by Shinan on 2019/1/18.
//
//可以相互通信的task的基类，子类只需要实现onMessage函数做消息处理，
//实现onStart,onStop管理生命周期
//
#ifndef SERVICEFRAMEWORK_INTERACTIVETASK_H
#define SERVICEFRAMEWORK_INTERACTIVETASK_H

#include <string>
#include <memory>
#include <atomic>
#include <unordered_map>
#include <logging.h>
#include "Task.h"
#include "ConcurrentRingBuffer.h"
#include "Message.h"
#include "network/EventReactor.h"

using namespace std;

class InteractiveTask : public Task, public enable_shared_from_this<InteractiveTask>
{
  typedef ConcurrentRingBuffer<shared_ptr<Message>> MsgQueue;
  enum stat{STARTING, RUNNING, STOPPING, STOPPED};
public:
  InteractiveTask(const string &name);
  virtual ~InteractiveTask();
  void start();
  int notifyMsg(const shared_ptr<Message> &spMessage);
  const int getStat() const;
  void setStat(int stat);

protected:
  virtual bool _onStart() = 0;
  virtual void _onStop() = 0;
  virtual void _onMessage(const shared_ptr<Message> &spMessage) = 0; //接收Task间消息的处理函数
  void _setEventReactor(const shared_ptr<EventReactor> &eventReactor_);
  int _sendMsgTo(const string &taskName, const shared_ptr<Message> &spMessage);

private:
  void __stop();
  void __onMessage(int fd, short event);
  bool __setThreadName(const string &name);
  int __createEventFd();
  bool __registToMsgCenter();
  void __unregistToMsgCenter();
 // void __readLoop();

public:
  std::shared_ptr<EventReactor> eventReactor_;

private:
  std::shared_ptr<MsgQueue> recvMsgQueue_;//每个InteractiveTask独占一个消息队列，生命周期和task一致
  int eventFd_;
  atomic<int> stat_; //表示状态
};

#endif //SERVICEFRAMEWORK_INTERACTIVETASK_H
