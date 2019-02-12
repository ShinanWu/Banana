//
// Created by Shinan on 2019/1/9.
//线程安全单例模式，通过MessageCenter很容易进行task间通信，
// 如果频繁通信一定要getTaskRef()拿到目标对象直接notify。

#include <mutex>
#include <logging.h>
#include <assert.h>
#include "MessageCenter.h"

mutex MessageCenter::mtx_;
atomic<MessageCenter*> MessageCenter::pMessageCenter_(nullptr);

MessageCenter *MessageCenter::Instance()
{
  MessageCenter *pMsgCenter = pMessageCenter_.load(memory_order_acquire);
  if (pMsgCenter == nullptr)
  {
    lock_guard<mutex> lck(mtx_);
    pMsgCenter = pMessageCenter_.load(memory_order_acquire);
    if (pMsgCenter == nullptr)
    {
      pMsgCenter = new MessageCenter();
      pMessageCenter_.store(pMsgCenter, memory_order_release);
    }
  }
  return pMsgCenter;
}

bool MessageCenter::registerTask(const string &taskName, const weak_ptr<InteractiveTask> wpTask)
{
  return taskMap_.insert(taskName, wpTask);
}

void MessageCenter::unregisterTask(const string &taskName)
{
  taskMap_.erase(taskName);
}

int MessageCenter::syncSendMsgTo(const string &receTask, const shared_ptr<Message> &spMessage)
{
  weak_ptr<InteractiveTask> wpTask;
  if (!taskMap_.find(receTask, wpTask))
  {
    LOG(ERROR) << "Task " << receTask << " not registered!";
    return RECEIVER_NOT_EXIST; //未注册
  }

  auto spTask = wpTask.lock(); //线程安全的类型提升
  if (spTask == nullptr)
  {
    LOG(ERROR) << "Task " << receTask << " unregistered!";
    return RECEIVER_NOT_EXIST;
  }

  spTask->notifyMsg(spMessage);
  return RECEIVER_RECV_OK;
}

bool MessageCenter::asyncSendMsgTo(const string &receTask, const shared_ptr<Message> &spMessage)
{
  //to-do,暂未实现
  return false;
}

bool MessageCenter::getTaskRef(const string &receTask, weak_ptr<InteractiveTask> &wpTask)
{
  if (!taskMap_.find(receTask, wpTask))
  {
    LOG(ERROR) << "Task " << receTask << " not registered!";
    return RECEIVER_NOT_EXIST; //未注册
  }
  return true;
}
