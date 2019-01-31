//
// Created by Shinan on 2019/1/23.
//

#include "LibeventReactor.h"
#include "examples/StringStreamParser.h"
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
    if(pMessageEvent_)
      return false;
    struct event *pMessageEvent = event_new(pBase_, fd, EV_READ|EV_PERSIST, onEvent, pCallback);
    if (pMessageEvent == nullptr)
    {
      LOG(ERROR) << "create event failed!";
      return false;
    }
    if (event_add(pMessageEvent, nullptr) < 0)
    {
      LOG(ERROR) << "add event failed!";
      return false;
    }
    pMessageEvent_ = pMessageEvent;
    pMessageCallback_ = pCallback;

  }
  else if(event & EVENT_READ)  //读事件
  {
    if(eventBundleMap_[fd].pReadCallback_)
      return false;

    struct event *pReadEvent = event_new(pBase_, fd, EV_READ|EV_PERSIST, onEvent, pCallback);
    if (pReadEvent == nullptr)
    {
      LOG(ERROR) << "create EV_READ failed!";
      return false;
    }
    if (event_add(pReadEvent, nullptr) < 0)
    {
      LOG(ERROR) << "add EV_READ failed!";
      return false;
    }
    eventBundleMap_[fd].pReadEvent_ = pReadEvent;
    eventBundleMap_[fd].pReadCallback_ = pCallback;
    eventBundleMap_[fd].isReadEnabled = true;
  }
  else if(event & EVENT_WRITE)  //写事件
  {
    if(eventBundleMap_[fd].pWriteCallback_)
      return false;

    struct event *pWriteEvent = event_new(pBase_, fd, EV_WRITE, onEvent, pCallback);
    if (pWriteEvent == nullptr)
    {
      LOG(ERROR) << "create EV_WRITE failed!";
      return false;
    }
    if (event_add(pEvent, nullptr) < 0)
    {
      LOG(ERROR) << "add EV_WRITE failed!";
      return false;
    }
    eventBundleMap_[fd].pWriteEvent_ = pWriteEvent;
    eventBundleMap_[fd].pWriteCallback_ = pCallback;
    eventBundleMap_[fd].isWriteEnabled = true;
  }

}

bool LibeventRector::enableEvent(int fd, short events)
{
  if(fd < 0){assert(0);}
  if(events & EVENT_WRITE && eventBundleMap_[fd].pWriteEvent_)
  {
   if(!eventBundleMap_[fd].isWriteEnabled)
   {
     if(event_add(eventBundleMap_[fd].pWriteEvent_, nullptr) < 0)
     {
       assert(0);
     }
     eventBundleMap_[fd].isWriteEnabled = true;
   }
  }
  if(events |= EVENT_READ && eventBundleMap_[fd].pReadEvent_)
  {
    if(!eventBundleMap_[fd].isReadEnabled)
    {
      if(event_add(eventBundleMap_[fd].pReadEvent_, nullptr) < 0)
      {
        assert(0);
      }
      eventBundleMap_[fd].isReadEnabled = true;
    }
  }
  return true;
}

bool LibeventRector::disableEvent(int fd, short events)
{
  if(fd < 0) assert(0);
  if(events & EVENT_READ && eventBundleMap_[fd].pReadEvent_)
  {
    if(event_del(eventBundleMap_[fd].pReadEvent_) < 0)
    {
      assert(0);
    }
    eventBundleMap_[fd].isReadEnabled = false;
  }
  if(events & EVENT_WRITE && eventBundleMap_[fd].pWriteEvent_)
  {
    if(event_del(eventBundleMap_[fd].pWriteEvent_) < 0)
    {
      assert(0);
    }
    eventBundleMap_[fd].isWriteEnabled = false;
  }

  return true;
}

bool LibeventRector::removeEventHandler(int fd, short event, const EventReactor::EventCallback &cb)
{
  if(fd < 0) assert(0);
  if(events & EVENT_READ && eventBundleMap_[fd].pReadEvent_)
  {
    if(event_del(eventBundleMap_[fd].pReadEvent_) < 0)
    {
      assert(0);
    }
    event_free(eventBundleMap_[fd].pReadEvent_);
    eventBundleMap_[fd].pReadEvent_ = nullptr;

    if(eventBundleMap_[fd].pReadCallback_)
    {
      delete eventBundleMap_[fd].pReadCallback_;
      eventBundleMap_[fd].pReadCallback_ = nullptr;
    }
    eventBundleMap_[fd].isReadEnabled = false;
  }
  if(events & EVENT_WRITE && eventBundleMap_[fd].pWriteEvent_)
  {
    if(event_del(eventBundleMap_[fd].pWriteEvent_) < 0)
    {
      assert(0);
    }
    event_free(eventBundleMap_[fd].pWriteEvent_);
    eventBundleMap_[fd].pWriteEvent_ = nullptr;

    if(eventBundleMap_[fd].pWriteCallback_)
    {
      delete eventBundleMap_[fd].pWriteCallback_;
      eventBundleMap_[fd].pWriteCallback_ = nullptr;
    }
    eventBundleMap_[fd].isWriteEnabled = false;
  }

  return true;
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

void LibeventRector::onEvent(evutil_socket_t fd, short event, void *pCallback)
{
  auto pCall = static_cast<EventCallback *> (pCallback);
  (*pCall)(fd, event);
}



void LibeventRector::startEventLoop()
{
  assert(pBase_ != nullptr);
  event_base_dispatch(pBase_);
}


