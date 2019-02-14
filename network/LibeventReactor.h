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
#define MAX_FD_NUM 50000 //默认最多5W连接
  typedef struct _EventBundle //事件相关数据
  {
    bool isWriteEnabled = false;
    bool isReadEnabled = false;
    struct event *pWriteEvent_ = nullptr;
    struct event *pReadEvent_ = nullptr;
    EventCallback *pWriteCallback_ = nullptr;
    EventCallback *pReadCallback_ = nullptr;
    ~_EventBundle()
    {
      if(pWriteEvent_){if(pWriteEvent_) event_free(pWriteEvent_); pWriteEvent_ = nullptr;}
      if(pReadEvent_){if(pReadEvent_) event_free(pReadEvent_); pReadEvent_ = nullptr;}
      if(pWriteCallback_){if(pWriteCallback_) delete pWriteCallback_; pWriteCallback_ = nullptr;}
      if(pReadCallback_){if(pReadCallback_) delete pReadCallback_; pReadCallback_ = nullptr;}
    }
  }EventBundle;

  LibeventRector();
  ~LibeventRector();
public:
  virtual bool initReactor(int maxFds = MAX_FD_NUM);
  virtual void destroyReactor();
  virtual bool bindPort(unsigned short port);
  virtual bool addEventHandler(int fd, short event, const EventCallback &cb);
  virtual bool enableEvent(int fd, short events);
  virtual bool disableEvent(int fd, short events);
  virtual bool removeEventHandler(int fd, short event, const EventCallback &cb);
  virtual void startEventLoop();

private:
  struct event_base *pBase_;
  struct evconnlistener *pListener_;
  struct sockaddr_in sin_;
  EventCallback *pAcceptCallback_;
  EventCallback *pMessageCallback_;
  struct event *pMessageEvent_;
  EventBundle* eventBundleMap_;

private:
  static void __onAccept(struct evconnlistener *listener,
                         evutil_socket_t fd,
                         struct sockaddr *addr,
                         int len,
                         void *pCallback);
  static void __onEvent(evutil_socket_t fd, short event, void *pCallback);
};

#endif //SEVICEFRAMEWORK_LIBEVENTOBJ_H
