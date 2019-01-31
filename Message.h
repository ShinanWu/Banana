//
// Created by Shinan on 2019/1/19.
//

#ifndef SEVICEFRAMEWORK_MESSAGES_H
#define SEVICEFRAMEWORK_MESSAGES_H

#include <string>
#include <functional>
using namespace std;

class Message
{
public:
  typedef function<void(bool ret, Message)>
public:
  Message();
  ~Message();

private:
  string what_; //消息标识，决定向下转的类型

};

#endif //SEVICEFRAMEWORK_MESSAGES_H
