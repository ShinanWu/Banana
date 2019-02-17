//
// Created by Shinan on 2019/2/16.
//

#ifndef SERVICEFRAMEWORK_NEWCONNECTMESSAGE_H
#define SERVICEFRAMEWORK_NEWCONNECTMESSAGE_H

#include "Message.h"

class NewConnectMessage : public Message
{
public:
  NewConnectMessage(int clientFd) :Message("NEW_CONNECT"),clientFd_(clientFd){}
  virtual ~NewConnectMessage(){}
  int getClientFd(){return clientFd_;}
private:
  int clientFd_;
};

#endif //SERVICEFRAMEWORK_NEWCONNECTMESSAGE_H
