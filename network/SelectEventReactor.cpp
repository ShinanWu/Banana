//
// Created by Shinan on 2019/2/1.
//

#include "SelectEventReactor.h"
bool SelectEventReactor::createReactor(int maxFds)
{
  return EventReactor::createReactor(maxFds);
}
void SelectEventReactor::destroyReactor()
{
  EventReactor::destroyReactor();
}
bool SelectEventReactor::addEventHandler(int fd, short event, const EventReactor::EventCallback &cb)
{
  return EventReactor::addEventHandler(fd, event, cb);
}
bool SelectEventReactor::enableEvent(int fd, short events)
{
  return EventReactor::enableEvent(fd, events);
}
bool SelectEventReactor::disableEvent(int fd, short events)
{
  return EventReactor::disableEvent(fd, events);
}
bool SelectEventReactor::removeEventHandler(int fd, short event, const EventReactor::EventCallback &cb)
{
  return EventReactor::removeEventHandler(fd, event, cb);
}
void SelectEventReactor::startEventLoop()
{
  EventReactor::startEventLoop();
}
