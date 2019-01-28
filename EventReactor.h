//
// Created by Shinan on 2019/1/26.
//

#ifndef SEVICEFRAMEWORK_EVENTREACTOR_H
#define SEVICEFRAMEWORK_EVENTREACTOR_H
#include <functional>
#include "Connection.h"
#include "Task.h"
#include <functional>

using namespace std;

//事件发生器基类
class EventReactor
{
public:
  enum _EVENT
  {
    EVENT_ACCEPT = 1 << 0, //accept事件
    EVENT_WRITE = 1 << 1,
    EVENT_READ = 1 << 2,
    EVENT_MESSAGE = 1 << 3, //内部消息事件
    EVENT_TIMEOUT = 1 << 4
  };
  typedef std::function<void(int fd, short event)> EventCallback;
public:
  virtual bool createReactor(int maxFds);
  virtual void destroyReactor();
  virtual bool bindPort(unsigned short port);
  virtual bool addEventHandler(int fd, short event, const EventCallback& cb);
  virtual bool removeEventHandler(int fd, short event, const EventCallback& cb);
  virtual bool freeEvenrHandler(int fd, short event, const EventCallback& cb)
  virtual void startEventLoop();
};

#endif //SEVICEFRAMEWORK_EVENTREACTOR_H


