//
// Created by Shinan on 2019/1/19.
//消息基类，整个框架以此消息结构为基础，可设置回调函数返回应答消息

#ifndef SEVICEFRAMEWORK_MESSAGES_H
#define SEVICEFRAMEWORK_MESSAGES_H

#include <string>
#include <functional>
using namespace std;

class Message
{
public:
  typedef function<void(bool ret, const Message& retMessage)> MessageCallBack;
public:
  Message(const string &what_, const MessageCallBack &messageCallBack_);
  ~Message();

  bool getWhat_(string &what) const;
  bool getMsgCallBack(MessageCallBack &msgCallBack) const;

private:
  string what_; //消息标识，决定向下转的类型和唯一标识
  MessageCallBack messageCallBack_;
};

#endif //SEVICEFRAMEWORK_MESSAGES_H
