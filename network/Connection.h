//
// Created by Shinan on 2019/1/27.
//

#ifndef SEVICEFRAMEWORK_CONNECTION_H
#define SEVICEFRAMEWORK_CONNECTION_H
#include <examples/StringStreamParser.h>
#include "Stream.h"
#include "network/EventReactor.h"

class Connection
{
 public:
  Connection(int fd);
  ~Connection();
  void onRead();
  void onWrite();
 private:
  struct event *pEvent_;
  StringStreamParser stream_;
  EventReactor* reactor_;

 public:
  struct event *getPEvent_() const;
  const StringStreamParser &getStream_() const;
  void setPEvent_(event *pEvent_);
  EventReactor *getReactor_() const;
  void setReactor_(EventReactor *reactor_);
};

#endif //SEVICEFRAMEWORK_CONNECTION_H
