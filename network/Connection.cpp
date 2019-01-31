//
// Created by Shinan on 2019/1/27.
//

#include "Connection.h"
struct event* Connection::getPEvent_() const
{
  return pEvent_;
}
void Connection::setPEvent_(struct event* pEvent)
{
  pEvent_ = pEvent;
}
const StringStreamParser &Connection::getStream_() const
{
  return stream_;
}
EventReactor *Connection::getReactor_() const
{
  return reactor_;
}
void Connection::setReactor_(EventReactor *reactor_)
{
  Connection::reactor_ = reactor_;
}
