//
// Created by Shinan on 2019/1/18.
//
//可以相互通信的task的基类，子类只需要实现onMessage函数做消息处理，
//accept向worker发送连接即是此方式。
//实现onStart,onStop管理生命周期。
//注意继承此基类不能用unique_ptr
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
  InteractiveTask(const string &name, const SpEventReactor &spEventReactor = nullptr);
  virtual ~InteractiveTask();
  void start() final ;
  int notifyMsg(const shared_ptr<Message> &spMessage);
  const int getStat() const;
  int getEventFd() const;
  void setStat(int stat);
  const SpEventReactor &getSpEventReactor() const;

protected:
  int sendMsgTo(const string &taskName, const shared_ptr<Message> &spMessage);
  virtual void onStart() = 0;
  virtual void onStop() = 0;
  virtual void onMessage(const shared_ptr<Message> &spMessage) = 0; //接收Task间消息的处理函数

private:
  void __stop();
  void __onMessage(int fd, short event);
  bool __registToMsgCenter();
  void __unregistToMsgCenter();


protected:
  SpEventReactor spEventReactor_{nullptr};

private:
  MsgQueue recvMsgQueue_{3000};//每个InteractiveTask独占一个消息队列，生命周期和task一致
  int eventFd_{-1};
  atomic<int> stat_{STARTING}; //表示状态
};

#endif //SERVICEFRAMEWORK_INTERACTIVETASK_H
