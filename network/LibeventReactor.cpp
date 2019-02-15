//
// Created by Shinan on 2019/1/23.
//

#include "LibeventReactor.h"
#include "Stream.h"
#include <logging.h>
#include <memory>
#include <stdlib.h>
#include <assert.h>

#define BACKLOG -1

LibeventRector::LibeventRector()
{

}

LibeventRector::~LibeventRector()
{
  destroyReactor();
}

bool LibeventRector::initReactor(int maxFds)
{
  assert(maxFds > 0);
  pBase_ = event_base_new();
  if (pBase_ == nullptr)
  {
    LOG(ERROR) << "create event base failed!";
    return false;
  }
  pEventBundleMap_ = new EventBundle[maxFds]();
  if (pEventBundleMap_ == nullptr)
  {
    return false;
  }
  return true;
}

void LibeventRector::destroyReactor()
{
  if (pBase_) { event_base_free(pBase_); pBase_ = nullptr; }
  if (pListener_) { evconnlistener_free(pListener_); pListener_ = nullptr; }
  if (pAcceptCallback_) { delete pAcceptCallback_; }
  if (pEventBundleMap_) { delete[] pEventBundleMap_; }
}

bool LibeventRector::bindPort(unsigned short port)
{
  sin_.sin_family = AF_INET;
  sin_.sin_addr.s_addr = htonl(0);//监听所有IP
  sin_.sin_port = htons(port);
  return true;
}

bool LibeventRector::addEventHandler(int fd, short events, const EventReactor::EventCallback &cb)
{
  if (fd < 0)
    assert(0);
  if (events & EVENT_ACCEPT && !pAcceptCallback_) //注册accept回调函数
  {
    pAcceptCallback_ = new EventCallback(std::move(cb));
    pListener_ = evconnlistener_new_bind(pBase_,
                                         __onAccept,
                                         (void *) pAcceptCallback_,
                                         LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                         BACKLOG,
                                         (struct sockaddr *) &sin_,
                                         sizeof(sin_));
    if (pListener_ == nullptr)
    {
      LOG(ERROR) << "create listener failed!";
      return false;
    }
  }
  if (events & EVENT_READ)  //读事件
  {
    if (!pEventBundleMap_[fd].pReadEvent_)
    {
      pEventBundleMap_[fd].pReadEvent_ = event_new(pBase_, fd, EV_READ, __onEvent, pEventBundleMap_);
      if (!pEventBundleMap_[fd].pReadEvent_)
      {
        LOG(ERROR) << "create EV_READ failed!";
        return false;
      }
    }
    if (event_add(pEventBundleMap_[fd].pReadEvent_, nullptr) < 0)
    {
      LOG(ERROR) << "add EV_READ failed!";
      return false;
    }
    pEventBundleMap_[fd].readCallback_ = std::move(cb);
  }
  if (events & EVENT_WRITE)  //写事件
  {
    if (!pEventBundleMap_[fd].pWriteEvent_)
    {
      pEventBundleMap_[fd].pWriteEvent_ = event_new(pBase_, fd, EV_WRITE, __onEvent, pEventBundleMap_);
      if (!pEventBundleMap_[fd].pWriteEvent_)
      {
        LOG(ERROR) << "create EV_WRITE failed!";
        return false;
      }
    }
    if (event_add(pEventBundleMap_[fd].pWriteEvent_, nullptr) < 0)
    {
      LOG(ERROR) << "add EV_WRITE failed!";
      return false;
    }
    pEventBundleMap_[fd].writeCallback_ = std::move(cb);
  }
  else
  {
    assert(0);
    return false;
  }
  return true;
}

bool LibeventRector::enableEvent(int fd, short events)
{
  if (fd < 0) assert(0);
  if (events & EVENT_WRITE && pEventBundleMap_[fd].pWriteEvent_)
  {
    if (event_add(pEventBundleMap_[fd].pWriteEvent_, nullptr) < 0)
    {
      assert(0);
    }
  }
  if (events & EVENT_READ && pEventBundleMap_[fd].pReadEvent_)
  {
    if (event_add(pEventBundleMap_[fd].pReadEvent_, nullptr) < 0)
    {
      assert(0);
    }
  }
  return true;
}

bool LibeventRector::disableEvent(int fd, short events)
{
  if (fd < 0) assert(0);
  if (events & EVENT_READ && pEventBundleMap_[fd].pReadEvent_)
  {
    if (event_del(pEventBundleMap_[fd].pReadEvent_) < 0)
    {
      assert(0);
    }
  }
  if (events & EVENT_WRITE && pEventBundleMap_[fd].pWriteEvent_)
  {
    if (event_del(pEventBundleMap_[fd].pWriteEvent_) < 0)
    {
      assert(0);
    }
  }
  return true;
}

bool LibeventRector::removeEventHandler(int fd, short events)
{
  if (fd < 0) assert(0);
  if (events & EVENT_READ && pEventBundleMap_[fd].pReadEvent_)
  {
    if (event_del(pEventBundleMap_[fd].pReadEvent_) < 0)
    {
      assert(0);
    }
    event_free(pEventBundleMap_[fd].pReadEvent_);
    pEventBundleMap_[fd].pReadEvent_ = nullptr;
  }
  if (events & EVENT_WRITE && pEventBundleMap_[fd].pWriteEvent_)
  {
    if (event_del(pEventBundleMap_[fd].pWriteEvent_) < 0)
    {
      assert(0);
    }
    event_free(pEventBundleMap_[fd].pWriteEvent_);
    pEventBundleMap_[fd].pWriteEvent_ = nullptr;
  }
  return true;
}

void LibeventRector::__onAccept(evconnlistener *listener,
                                evutil_socket_t fd,
                                struct sockaddr *addr,
                                int len,
                                void *pAcceptCallback)
{
  auto pAcceptCall = static_cast<EventCallback *> (pAcceptCallback);
  (*pAcceptCall)(fd, EVENT_ACCEPT);
}

void LibeventRector::__onEvent(evutil_socket_t fd, short event, void *pEventBundleMap)
{

  if (event == EV_WRITE)
  {
    auto pBundleMap = static_cast<EventBundle *> (pEventBundleMap);
    auto writeCallback = std::move(pBundleMap[fd].writeCallback_);
    assert(writeCallback);
    writeCallback(fd, EVENT_WRITE);
  }
  else if (event == EV_READ)
  {
    auto pBundleMap = static_cast<EventBundle *> (pEventBundleMap);
    auto readCallback = std::move(pBundleMap[fd].readCallback_);
    assert(readCallback);
    readCallback(fd, EVENT_READ);
  }
  else
  {
    //time-out event
  }
}

void LibeventRector::startEventLoop()
{
  assert(pBase_ != nullptr);
  event_base_dispatch(pBase_);
}


