//
// Created by Shinan on 2019/1/23.
//

#ifndef SEVICEFRAMEWORK_LIBEVENTOBJ_H
#define SEVICEFRAMEWORK_LIBEVENTOBJ_H

#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <string>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
//#include "Task.h"
#include "network/EventReactor.h"
//#include "Connection.h"



using namespace std;

class LibeventRector : public EventReactor
{
public:
  typedef struct _EventBundle //事件相关数据
  {
    struct event *pWriteEvent_ = nullptr;
    struct event *pReadEvent_ = nullptr;
    EventCallback writeCallback_ = nullptr;
    EventCallback readCallback_ = nullptr;
    ~_EventBundle()
    {
      if(pWriteEvent_){if(pWriteEvent_) event_free(pWriteEvent_); pWriteEvent_ = nullptr;}
      if(pReadEvent_){if(pReadEvent_) event_free(pReadEvent_); pReadEvent_ = nullptr;}
    }
  }EventBundle;

  LibeventRector();
  ~LibeventRector();
public:
  virtual bool initReactor(int maxFds);
  virtual void destroyReactor();
  virtual bool bindPort(unsigned short port);
  virtual bool addEventHandler(int fd, short events, const EventCallback &cb);
  virtual bool enableEvent(int fd, short events);
  virtual bool disableEvent(int fd, short events);
  virtual bool removeEventHandler(int fd, short events);
  virtual void startEventLoop();

private:
  struct event_base *pBase_ = nullptr;
  struct evconnlistener *pListener_ = nullptr;
  struct sockaddr_in sin_;
  EventBundle* pEventBundleMap_= nullptr ;
  EventCallback* pAcceptCallback_= nullptr;


private:
  static void __onAccept(struct evconnlistener *listener,
                         evutil_socket_t fd,
                         struct sockaddr *addr,
                         int len,
                         void *pAcceptCallback);
  static void __onEvent(evutil_socket_t fd, short event, void *pEventBundleMap);
};

#endif //SEVICEFRAMEWORK_LIBEVENTOBJ_H
