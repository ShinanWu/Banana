//
// Created by Shinan on 2019/1/18.
//
//可以相互通信的task的基类，子类需要设置线程名和实现相关函数。
//
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
  InteractiveTask(const string &name);
  virtual void start() final;
  virtual bool init();
  virtual void run() final;
  virtual void onMessage(const shared_ptr<Message> &spMessage) = 0; //接收消息的处理函数
  void onAccept(int fd);
  virtual handle

  const string &getTaskName_() const;
  void setTaskName_(const string &taskName_);
  bool waitEvent();

 private:
  bool _setThreadName(const string &name);

 private:
  std::shared_ptr<MsgQueue> recvMsgQueue_;//每个InteractiveTask独占一个消息队列，生命周期和task一致
  std::shared_ptr<EventReactor> eventReactor_;
  std::string taskName_;
  int eventFd_;
};

#endif //SERVICEFRAMEWORK_INTERACTIVETASK_H
