//
// Created by Shinan on 2019/1/9.
//

#ifndef SEVICEFRAMEWORK_MESSAGECENTER_H
#define SEVICEFRAMEWORK_MESSAGECENTER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <atomic>
#include "Message.h"
#include "ConcurrentRingBuffer.h"
#include "ConcurrentHashMap.h"
#include "InteractiveTask.h"

using namespace std;

class MessageCenter
{
public:
  enum _ErrorType
  {
    RECEIVER_RECV_OK,
    RECEIVER_NOT_EXIST, //接受者未注册
    RECEIVER_BUSY,      // 接收的队列已满
    UNKNOWN_ERR
  };
  typedef ConcurrentHashMap<string, weak_ptr<InteractiveTask>> TaskMap;//每个InteractiveTask独占一个消息队列，生命周期和task一致

public:
  static MessageCenter* Instance();
  bool registerTask(const string &taskName, const weak_ptr<InteractiveTask> wpTask);//注册task
  void unregisterTask(const string &taskName);                                   //注销
  int syncSendMsgTo(const string &receTask, const shared_ptr<Message> &spMessage);//同步发送消息
  bool asyncSendMsgTo(const string &receTask, const shared_ptr<Message> &spMessage); //异步发送消息
  bool getTaskRef(const string &receTask, weak_ptr<InteractiveTask> &wpTask); //如果频繁发送消息建议直接获取接收端task的引用，不要每次通过MessageCenter

private:
  TaskMap taskMap_;
  static mutex mtx_;
  static atomic<MessageCenter*> pMessageCenter_;//？？？
};

#endif //SEVICEFRAMEWORK_MESSAGECENTER_H
