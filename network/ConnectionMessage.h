//
// Created by Shinan on 2019/2/18.
//

#ifndef SERVICEFRAMEWORK_CONNECTIONMESSAGE_H
#define SERVICEFRAMEWORK_CONNECTIONMESSAGE_H

#include "multi-threading/Message.h"
#include <memory>

class ConnectionMessage : public Message
{
public:
  ConnectionMessage(int handle) : Message("CONNECTION"), handle_(handle)
  {}

  int getHandle()
  { return handle_; }

private:
  int handle_;
};
typedef std::shared_ptr<ConnectionMessage> SpConnectionMessage;

#endif //SERVICEFRAMEWORK_CONNECTIONMESSAGE_H
