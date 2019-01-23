//
// Created by Shinan on 2019/1/18.
//
//可以相互通信的task的基类，子类需要设置线程名和实现相关函数。
//
//
#ifndef SERVICEFRAMEWORK_INTERACTIVETASK_H
#define SERVICEFRAMEWORK_INTERACTIVETASK_H

#include <string>
#include <memory>
#include <unordered_map>
#include <logging.h>
#include "Task.h"
#include "ConcurrentRingBuffer.h"
#include "Message.h"

using namespace std;

class InteractiveTask : public Task{
    typedef ConcurrentRingBuffer<shared_ptr<Message>> MsgQueue;
    typedef function<void(int eventFd, int event)> EventCallback;

public:
    InteractiveTask(const string& name);
    void start();
    virtual bool init() = 0;
    virtual void onMessage(const shared_ptr<Message>& spMessage) = 0; //接收消息的处理函数

    const string &getTaskName_() const;
    void setTaskName_(const string &taskName_);
    virtual bool createReactor(); //自行选择select、poll和epoll，如果只实现onTaskMessage则阻塞read(fd)即可。
    //int createEventFd(); //创建eventFd
    virtual bool addEvent(int eventFd, int event, EventCallback eventCallback, );//添加事件和回调函数
    //bool waitEvent();

private:
    bool _setThreadName(const string& name);

private:
    std::shared_ptr<MsgQueue> recvMsgQueue_;//每个InteractiveTask独占一个消息队列，生命周期和task一致
    std::string taskName_;
    int eventFd_;

};

#endif //SERVICEFRAMEWORK_INTERACTIVETASK_H
