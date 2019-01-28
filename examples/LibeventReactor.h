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
#include "Task.h"
#include "EventReactor.h"
#include "Connection.h"

using namespace std;

class LibeventRector : public EventReactor
{
public:
#define MAX_FD_NUM 50000 //默认最多5W连接
  typedef struct
  {
    short events_;
    struct event *pEvent_;
    EventCallback *pWriteCallback_;
    EventCallback *pReadCallback_;
  }EventBundle;

  LibeventRector();
  ~LibeventRector();
public:
  virtual bool createReactor(int maxFds);
  virtual void destroyReactor();
  virtual bool bindPort(unsigned short port);
  virtual bool addEventHandler(int fd, short event, const EventCallback &cb);
  virtual bool enableEvent(int fd, short events);
  virtual bool disableEvent(int fd, short events);
  virtual bool removeEventHandler(int fd, short event, const EventCallback &cb);
 // virtual bool freeEvenrHandler(int fd, short event, const EventCallback &cb)
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
  static void onAccept(struct evconnlistener *listener,
                       evutil_socket_t fd,
                       struct sockaddr *addr,
                       int len,
                       void *pCallback);
  static void onRead(evutil_socket_t fd, short events, void *pCallback);
  static void onWrite(evutil_socket_t fd, short events, void *pCallback);
  static void onMessage(evutil_socket_t fd, short events, void *pCallback);

};

#endif //SEVICEFRAMEWORK_LIBEVENTOBJ_H
