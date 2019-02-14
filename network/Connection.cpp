//
// Created by Shinan on 2019/1/27.
//

#include "Connection.h"
#include "Stream.h"

Connection::Connection(int fd,
                       struct event *pEvent,
                       const SpEventReactor &spEventReactor)
    : pEvent_(pEvent_)
    , stringStream_(fd)
    , spEventReactor_(spEventReactor)
{}

void Connection::onRead()
{
  StreamStat streamStat = stringStream_.recvOnePck();
  if(streamStat == TOCLOSE)
  {
    onClose();
    LOG(ERROR) << "stream closed";
    return;
  }
  else if(streamStat == RECVED)
  {
    string message;
    stringStream_.getPck(message);
    onMessage(message);
  }
  else
  {
    return;
  }
}
void Connection::onWrite()
{
  if(stringStream_.sendPck() == TOCLOSE)
  {
    onClose();
  }
}
void Connection::onClose()
{

}
void Connection::onMessage(const string &message)
{

}
