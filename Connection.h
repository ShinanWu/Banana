//
// Created by Shinan on 2019/1/27.
//

#ifndef SEVICEFRAMEWORK_CONNECTION_H
#define SEVICEFRAMEWORK_CONNECTION_H
#include <examples/Stream.h>
#include "Stream.h"
#include "EventReactor.h"

class Connection
{
 public:
  Connection(int fd);
  ~Connection();
  static void onRead();
  static void onWrite();
 private:
  struct event *pEvent_;
  Stream stream_;
  EventReactor* reactor_;

 public:
  struct event *getPEvent_() const;
  const Stream &getStream_() const;
  void setPEvent_(event *pEvent_);
  EventReactor *getReactor_() const;
  void setReactor_(EventReactor *reactor_);
};

#endif //SEVICEFRAMEWORK_CONNECTION_H
