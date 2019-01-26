//
// Created by Shinan on 2019/1/23.
//

#include "LibeventReactor.h"
#include "Stream.h"
#include <memory>
#include <stdlib.h>

#define BACKLOG -1

LibeventRector::LibeventRector()
{

}

LibeventRector::~LibeventRector()
{
  destroyReactor();
}

bool LibeventRector::creatReactor()
{
  base_ = event_base_new();
  if(base_ == nullptr)
  {
    LOG(ERROR) << "create event base failed!";
    return false;
  }
  return true;
}

void LibeventRector::destroyReactor()
{
  if(base_)
  {
    event_base_free(base_);
    base_ = nullptr;
  }
  if(listener_)
  {
    evconnlistener_free(listener_);
    listener_ = nullptr;
  }
}

bool LibeventRector::bindPort(unsigned short port)
{
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(0);//监听所有IP
  sin.sin_port = htons(port);
  listener_ = evconnlistener_new_bind(base_, onAccept, (void*)this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                      BACKLOG, (struct sockaddr*)&sin, sizeof(sin));
  if(listener_ == nullptr)
  {
    LOG(ERROR) << "create listener failed!";
    return false;
  }
}

void LibeventRector::onAccept(int fd)
{
  Connection *connection = new Connection(fd);
  struct event* pEvent = event_new(base_, fd, EV_READ, onRead, connection);
  if(pEvent == nullptr)
  {
    LOG(ERROR) << "create event failed!";
    return;
  }
  connection->setPEvent_(pEvent);
  connection->setReactor_(this);

  if(event_add(pEvent, nullptr) < 0)
  {
    LOG(ERROR) << "add event failed!";
    return;
  }

}

bool LibeventRector::addEventHandler(short event, struct event* pEvent, Connection* handler)
{

}

void LibeventRector::onAccept(evconnlistener *listener,
                              evutil_socket_t fd,
                              struct sockaddr *addr,
                              int len,
                              void *pData)
{
  LibeventRector* pThis = static_cast<LibeventRector*> (pData);
  pThis->onAccept(fd);
}

bool LibeventRector::addEventHandler(Task *handler)
{
  return false;
}

void LibeventRector::onRead(evutil_socket_t fd, short events, void *pData)
{

}

void LibeventRector::onWrite(evutil_socket_t fd, short events, void *pData)
{

}

void LibeventRector::onMessage(evutil_socket_t fd, short events, void *pData)
{

}

void LibeventRector::startEventLoop()
{
  EventReactor::startEventLoop();
}