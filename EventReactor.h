//
// Created by Shinan on 2019/1/26.
//

#ifndef SEVICEFRAMEWORK_EVENTREACTOR_H
#define SEVICEFRAMEWORK_EVENTREACTOR_H
#include <functional>
#include "Connection.h"
#include "Task.h"

using namespace std;

//事件发生器基类
class EventReactor
{
  enum _Event{
    EVENT_WRITE,
    EVENT_READ,
    EVENT_MESSAGE
  };
 public:
  typedef function<void(int eventFd, short event)> EventCallback;
 public:
  virtual bool creatReactor();
  virtual void destroyReactor();
  virtual bool bindPort(unsigned short port);
  virtual void onAccept(int fd);
  virtual bool addEventHandler(Task* handler);
  virtual bool addEventHandler(short event, Connection* handler); // virtual bool removeEventHandler(EventCallback cb);
  virtual void startEventLoop();
};

#endif //SEVICEFRAMEWORK_EVENTREACTOR_H


