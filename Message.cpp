//
// Created by Shinan on 2019/1/19.
//

#include "Message.h"
#include <assert.h>

Message::Message()
{}
const string& Message::getWhat() const
{
  return what_;
}
bool Message::getMsgCallBack(MessageCallBack &msgCallBack) const
{
  msgCallBack = std::move(messageCallBack_);
  return true;
}
void Message::setWhat(const string &what)
{
  what_ = what;
}
void Message::setMessageCallBack(const MessageCallBack &messageCallBack)
{
  messageCallBack_ = std::move(messageCallBack);
}
