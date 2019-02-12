//
// Created by Shinan on 2019/2/1.
//

#include "SelectEventReactor.h"
bool SelectEventReactor::initReactor(int maxFds)
{
  return false;
}
void SelectEventReactor::destroyReactor()
{
}
bool SelectEventReactor::addEventHandler(int fd, short event, const EventReactor::EventCallback &cb)
{
  return false;
}
bool SelectEventReactor::enableEvent(int fd, short events)
{
  return false;
}
bool SelectEventReactor::disableEvent(int fd, short events)
{
  return false;
}
bool SelectEventReactor::removeEventHandler(int fd, short event, const EventReactor::EventCallback &cb)
{
  return false;
}
void SelectEventReactor::startEventLoop()
{
}
