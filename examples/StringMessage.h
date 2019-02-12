//
// Created by Shinan on 2019/2/1.
//字符串消息类型

#ifndef SEVICEFRAMEWORK_STRINGMESSAGE_H
#define SEVICEFRAMEWORK_STRINGMESSAGE_H
#include "Message.h"

class StringMessage : public Message
{
public:
  StringMessage(const string &what_, const MessageCallBack &messageCallBack_);
  void setData(const string &data);
  bool getData(string& data) const;
private:
  string data_;
};

StringMessage::StringMessage(const string &what, const Message::MessageCallBack &messageCallBack)
    : Message()
{}

bool StringMessage::getData(string& data) const
{
  data = std::move(data);
  return true;
}
void StringMessage::setData(const string &data)
{
  data_ = std::move(data);
}

#endif //SEVICEFRAMEWORK_STRINGMESSAGE_H
