//
// Created by Shinan on 2019/1/19.
//
#include <sys/prctl.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <assert.h>
#include <utils/Utils.h>
#include "InteractiveTask.h"
#include "MessageCenter.h"

#define NORMAL_THREAD_NAME "availible"

using namespace std::placeholders;

InteractiveTask::InteractiveTask(const string &name, const SpEventReactor &spEventReactor)
    : Task(name), spEventReactor_(spEventReactor)
{}

InteractiveTask::~InteractiveTask()
{}

void InteractiveTask::start()
{
  //设置线程名，方便调试
  assert(Utils::setThreadName(getTaskName()));
  LOG(INFO) << "Task " << getTaskName() << " started!";
  eventFd_ = Utils::createEventFd();
  assert(eventFd_ > 0);
  setStat(RUNNING);

  //有事件反应堆
  if (spEventReactor_)
  {
    assert(spEventReactor_->initReactor());
    spEventReactor_->addEventHandler(eventFd_, EventReactor::EVENT_READ,
                                     std::bind(&InteractiveTask::__onMessage, this, _1, _2));
    assert(__registToMsgCenter()); //向MessageCenter注册
    onStart();//子类行为

    //start Loop
    LOG(INFO) << "start loop!";
    spEventReactor_->startEventLoop();
  }
    //无事件反应堆则直接阻塞read
  else
  {
    assert(__registToMsgCenter());
    onStart();//子类行为

    //start Loop
    unsigned long long eventVal;
    while (getStat() == RUNNING)
    {
      read(eventFd_, &eventVal, sizeof(eventVal)); //内部counter会减1
      assert(eventVal == 1);
      shared_ptr<Message> message;
      if (recvMsgQueue_.syncGet(message))
      {
        onMessage(message);
      }
    }
  }
  //停止loop
  setStat(STOPPED);
  __stop();
}

void InteractiveTask::__stop() //在此释放资源，不在析构是因为可能智能指针在buffer不能及时释放
{
  onStop();//子类行为
  __unregistToMsgCenter();
  assert(Utils::setThreadName(NORMAL_THREAD_NAME));
  if (spEventReactor_)
  {
    spEventReactor_->destroyReactor();
  }
  if (eventFd_ > 0)
  {
    close(eventFd_);
    eventFd_ = -1;
  }
  LOG(INFO) << "Task " << getTaskName() << " stopped!";
}

void InteractiveTask::__onMessage(int fd, short event)
{
  assert(fd == eventFd_);
  unsigned long long eventVal;
  read(eventFd_, &eventVal, sizeof(eventVal)); //内部counter会减1
  assert(eventVal == 1);
  shared_ptr<Message> message;
  if (recvMsgQueue_.syncGet(message))
  {
    onMessage(message);
  }
  spEventReactor_->addEventHandler(eventFd_, EventReactor::EVENT_READ,
                                   std::bind(&InteractiveTask::__onMessage, this, _1, _2));
}

int InteractiveTask::sendMsgTo(const string &taskName, const shared_ptr<Message> &spMessage)
{
  return MessageCenter::Instance()->syncSendMsgTo(taskName, spMessage);
}

//线程安全
int InteractiveTask::notifyMsg(const shared_ptr<Message> &spMessage)
{
  recvMsgQueue_.syncPut(spMessage);
  unsigned long long eventVal = 1;
  write(eventFd_, &eventVal, sizeof(eventVal)); //内部counter会加1
  return 0;
}

const int InteractiveTask::getStat() const
{
  return stat_.load(memory_order_relaxed);
}

void InteractiveTask::setStat(int stat)
{
  stat_.store(stat, memory_order_relaxed);
}

//void InteractiveTask::_setEventReactor(const SpEventReactor &eventReactor_)
//{
//  InteractiveTask::spEventReactor_ = std::move(eventReactor_);
//}

bool InteractiveTask::__registToMsgCenter()
{
  if (!MessageCenter::Instance()->registerTask(getTaskName(), shared_from_this()))
  {
    LOG(ERROR) << "Task " << getTaskName() << " already exist!";
    return false;
  }
  return true;
}

void InteractiveTask::__unregistToMsgCenter()
{
  MessageCenter::Instance()->unregisterTask(getTaskName());
}

const SpEventReactor &InteractiveTask::getSpEventReactor() const
{
  return spEventReactor_;
}

int InteractiveTask::getEventFd() const
{
  return eventFd_;
}


