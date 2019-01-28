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

bool LibeventRector::createReactor(int maxFds)
{
  pBase_ = event_base_new();
  if (pBase_ == nullptr)
  {
    LOG(ERROR) << "create event base failed!";
    return false;
  }
  eventBundleMap_ = new EventBundle[maxFds]();
  if(eventBundleMap_ == nullptr)
  {
    return false;
  }
  return true;
}

void LibeventRector::destroyReactor()
{
  if (pBase_)
  {
    event_base_free(pBase_);
    pBase_ = nullptr;
  }
  if (pListener_)
  {
    evconnlistener_free(pListener_);
    pListener_ = nullptr;
  }
  if(eventBundleMap_)
  {
    delete[] eventBundleMap_;
  }
}

bool LibeventRector::bindPort(unsigned short port)
{
  sin_.sin_family = AF_INET;
  sin_.sin_addr.s_addr = htonl(0);//监听所有IP
  sin_.sin_port = htons(port);
  return true;
}

void LibeventRector::onAccept(int fd)
{
  Connection *connection = new Connection(fd);
  struct event *pEvent = event_new(pBase_, fd, EV_READ, onRead, connection);
  if (pEvent == nullptr)
  {
    LOG(ERROR) << "create event failed!";
    return;
  }
  connection->setPEvent_(pEvent);
  connection->setReactor_(this);

  if (event_add(pEvent, nullptr) < 0)
  {
    LOG(ERROR) << "add event failed!";
    return;
  }

}

bool LibeventRector::addEventHandler(int fd, short event, const EventReactor::EventCallback &cb)
{
  if(fd < 0)
    assert(0);
  auto pCallback = new EventCallback(cb);
  if(event & EVENT_ACCEPT && pListener_ == nullptr) //accept事件
  {
    pListener_ = evconnlistener_new_bind(pBase_, onAccept, (void *)pCallback, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                        BACKLOG, (struct sockaddr *) &sin_, sizeof(sin));
    if (pListener_ == nullptr)
    {
      LOG(ERROR) << "create listener failed!";
      return false;
    }
    pAcceptCallback_ = pCallback;
  }
  else if(event & EVENT_MESSAGE)
  {
    if(pMessageCallback_)
      return false;
    struct event *pEvent = event_new(pBase_, fd, EV_READ, onMessage, pCallback);
    if (pEvent == nullptr)
    {
      LOG(ERROR) << "create event failed!";
      return false;
    }
    if (event_add(pEvent, nullptr) < 0)
    {
      LOG(ERROR) << "add event failed!";
      return false;
    }
    pMessageCallback_ = pCallback;
    pMessageEvent_ = pEvent;
  }
  else if(event & EVENT_READ)  //读事件
  {
    if(eventBundleMap_[fd].pReadCallback_)
      return false;

    struct event *pEvent = event_new(pBase_, fd, EV_READ, onRead, pCallback);
    if (pEvent == nullptr)
    {
      LOG(ERROR) << "create event failed!";
      return false;
    }
    if (event_add(pEvent, nullptr) < 0)
    {
      LOG(ERROR) << "add event failed!";
      return false;
    }
    eventBundleMap_[fd].pEvent_ = pEvent;
    eventBundleMap_[fd].pReadCallback_ = pCallback;
  }
  else if(event & EVENT_WRITE)  //写事件
  {
    if(eventBundleMap_[fd].pWriteCallback_)
      return false;

    struct event *pEvent = event_new(pBase_, fd, EV_READ, onRead, pCallback);
    if (pEvent == nullptr)
    {
      LOG(ERROR) << "create event failed!";
      return false;
    }
    if (event_add(pEvent, nullptr) < 0)
    {
      LOG(ERROR) << "add event failed!";
      return false;
    }
    eventBundleMap_[fd].pEvent_ = pEvent;
    eventBundleMap_[fd].pWriteCallback_ = pCallback;
  }

}

bool LibeventRector::removeEventHandler(int fd, short event, const EventReactor::EventCallback &cb)
{

}

bool LibeventRector::freeEvenrHandler(int fd, short event, const EventReactor::EventCallback &cb)
{
  return EventReactor::freeEvenrHandler(fd, event, cb);
}

void LibeventRector::onAccept(evconnlistener *listener,
                              evutil_socket_t fd,
                              struct sockaddr *addr,
                              int len,
                              void *pCallback)
{
  auto pAcceptCall = static_cast<EventCallback *> (pCallback);
  (*pAcceptCall)(fd, EVENT_ACCEPT);
}

void LibeventRector::onRead(evutil_socket_t fd, short events, void *pCallback)
{
  auto pReadCall = static_cast<EventCallback *> (pCallback);
  (*pReadCall)(fd, events);
}

void LibeventRector::onWrite(evutil_socket_t fd, short events, void *pCallback)
{
  auto pWriteCall = static_cast<EventCallback *> (pCallback);
  (*pWriteCall)(fd, events);
}

void LibeventRector::onMessage(evutil_socket_t fd, short events, void *pCallback)
{
  auto pMessageCall = static_cast<EventCallback *> (pCallback);
  (*pMessageCall)(fd, events);
}

void LibeventRector::startEventLoop()
{
  EventReactor::startEventLoop();
}

