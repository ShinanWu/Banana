//
// Created by Shinan on 2019/1/18.
//
//可以相互通信的task的基类，子类需要设置线程名和实现start函数。
#ifndef SERVICEFRAMEWORK_INTERACTIVETASK_H
#define SERVICEFRAMEWORK_INTERACTIVETASK_H

#include <string>
#include <memory>
#include <unordered_map>
#include <logging.h>
#include "Task.h"
#include "ConcurrentRingBuffer.h"
#include "Message.h"



class InteractiveTask : public Task{
    typedef ConcurrentRingBuffer<shared_ptr<Message>> MsgQueue;

public:
    InteractiveTask(const string& name);
    void start();
    virtual void run() = 0;

    const string &getTaskName_() const;

    void setTaskName_(const string &taskName_);

private:
    bool _setThreadName(const string& name)

private:
    std::shared_ptr<MsgQueue> recvMsgQueue_;//每个InteractiveTask独占一个消息队列，生命周期和task一致
    std::string taskName_;

};

#endif //SERVICEFRAMEWORK_INTERACTIVETASK_H
