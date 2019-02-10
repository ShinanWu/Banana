//
// Created by Shinan on 2019/1/19.
//
#include <sys/prctl.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include "InteractiveTask.h"

#define NORMAL_THREAD_NAME "availible"

using namespace std::placeholders;

InteractiveTask::InteractiveTask(const string &name, std::shared_ptr<EventReactor> eventReactor)
    : Task(name), eventFd_(-1), eventReactor_(std::move(eventReactor))
{}

InteractiveTask::~InteractiveTask()
{}

void InteractiveTask::start()
{
  //设置线程名，方便调试
  assert(_setThreadName(getTaskName()));
  LOG(INFO) << "Task " << getTaskName() << " started!";
  assert(onStart());
  eventFd_ = _createEventFd();
  assert(eventFd_ > 0);
  //有事件反应堆
  if (eventReactor_)
  {
    assert(eventReactor_->initReactor());
    eventReactor_->addEventHandler(eventFd_, EventReactor::EVENT_MESSAGE,
                                   std::bind(&InteractiveTask::_onMessage, this, _1, _2));
  }
  //无事件反应堆则直接阻塞read就行
  else
  {
    unsigned long long eventVal;
    while (true)
    {
      read(eventFd_, &eventVal, sizeof(eventVal)); //内部counter会减1
      assert(eventVal == 1);
      shared_ptr<Message> message;
      if (recvMsgQueue_->syncGet(message))
      {
        onMessage(message);
      }
    }
    onStop();
  }
}

void InteractiveTask::stop()
{
  assert(_setThreadName(NORMAL_THREAD_NAME));
  if(eventReactor_) {eventReactor_->destroyReactor();}
  assert(onStop());
  LOG(INFO) << "Task " << getTaskName() << " stopped!";
}

void InteractiveTask::_onMessage(int fd, short event)
{
  assert(fd == eventFd_);
  unsigned long long eventVal;
  read(eventFd_, &eventVal, sizeof(eventVal)); //内部counter会减1
  assert(eventVal == 1);
  shared_ptr<Message> message;
  if (recvMsgQueue_->syncGet(message))
  {
    onMessage(message);
  }
}

bool InteractiveTask::_setThreadName(const string &name)
{
  if (prctl(PR_SET_NAME, name.c_str(), 0, 0, 0) == -1)
  {
    LOG(ERROR) << "set thread name failed! name:" << name;
    return false;
  }
  return true;
}
int InteractiveTask::_createEventFd()
{
  //不用设置非阻塞，因为信号量性质，有事件通知一定有事件，不会阻塞。没事件也不会来读
  int evtfd = eventfd(0, EFD_CLOEXEC | EFD_SEMAPHORE);
  if (evtfd < 0)
  {
    LOG(ERROR) << "Failed in eventfd";
    assert(0);
  }
  return evtfd;
}
int InteractiveTask::sendMsg(const string &taskName, const shared_ptr<Message> &spMessage)
{
  return 0;
}
int InteractiveTask::notifyMsg(const shared_ptr<Message> &spMessage)
{
  recvMsgQueue_->syncPut(spMessage);
  unsigned long long eventVal = 1;
  write(eventFd_, &eventVal, sizeof(eventVal)); //内部counter会加1
  return 0;
}

