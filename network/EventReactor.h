//
// Created by Shinan on 2019/1/26.
//

#ifndef SEVICEFRAMEWORK_EVENTREACTOR_H
#define SEVICEFRAMEWORK_EVENTREACTOR_H
#include <functional>
#include <memory>
#include "multi-threading/Task.h"
#define DEFAULT_MAX_CLIENTS 50000
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
    EVENT_TIMEOUT = 1 << 3
  };
  typedef std::function<void(int fd, short event)> EventCallback;

public:
  virtual bool initReactor(int maxFds=DEFAULT_MAX_CLIENTS)=0;
  virtual void destroyReactor()=0;
  virtual bool bindPort(unsigned short port)=0;
  virtual bool addEventHandler(int fd, short events, const EventCallback& cb)=0;
  virtual bool enableEvent(int fd, short events)=0;
  virtual bool disableEvent(int fd, short events)=0;
  virtual bool removeEventHandler(int fd, short events)=0;
  virtual void startEventLoop()=0;
};

typedef shared_ptr<EventReactor> SpEventReactor;

#endif //SEVICEFRAMEWORK_EVENTREACTOR_H


