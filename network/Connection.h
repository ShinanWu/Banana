//
// Created by Shinan on 2019/1/27.
//

#ifndef SEVICEFRAMEWORK_CONNECTION_H
#define SEVICEFRAMEWORK_CONNECTION_H
#include <memory>
#include <network/Stream.h>
//#include "Stream.h"
#include "network/EventReactor.h"

class Connection
{
public:
  Connection(int fd,
             struct event *pEvent,
             const SpEventReactor &spEventReactor);
  ~Connection();
  void onRead();
  void onWrite();
  void onMessage(const string &message);
  void onClose();
private:
  struct event *pEvent_;
  Stream stringStream_;
  SpEventReactor spEventReactor_;

public:
  struct event *getPEvent_() const;
  const Stream &getStream_() const;
};

#endif //SEVICEFRAMEWORK_CONNECTION_H
