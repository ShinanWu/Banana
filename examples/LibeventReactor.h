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

class LibeventRector : public EventReactor{
public:
    LibeventRector();
    ~LibeventRector();
 public:
  virtual bool creatReactor();
  virtual void destroyReactor();
  virtual bool bindPort(unsigned short port);
  virtual void onAccept(int fd);
  virtual bool addEventHandler(Task* handler);
  virtual bool addEventHandler(short event, Connection* handler);
 // virtual bool removeEventHandler(EventCallback cb);
  virtual void startEventLoop();

 private:
  struct event_base *base_;
  struct evconnlistener *listener_;
  unsigned short port_;
 private:
  static void onAccept(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *pData);
  static void onRead(evutil_socket_t fd, short events, void *pData);
  static void onWrite(evutil_socket_t fd, short events, void *pData);
  static void onMessage(evutil_socket_t fd, short events, void *pData);

};


#endif //SEVICEFRAMEWORK_LIBEVENTOBJ_H
