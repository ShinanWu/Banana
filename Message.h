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
  typedef function<void(const Message& retMessage)> MessageCallBack;
public:
  Message(const string &what = "UNKNOWN", const Message::MessageCallBack &messageCallBack = nullptr);

  const string& getWhat() const;
  bool getMsgCallBack(MessageCallBack &msgCallBack) const;
  void setWhat(const string &what_);
  void setMessageCallBack(const MessageCallBack &messageCallBack_);
private:
  string what_; //消息标识，决定向下转的类型和唯一标识
  MessageCallBack messageCallBack_;
};

#endif //SEVICEFRAMEWORK_MESSAGES_H
