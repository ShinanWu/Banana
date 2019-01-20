//
// Created by Shinan on 2019/1/9.
//

#ifndef SEVICEFRAMEWORK_MESSAGECENTER_H
#define SEVICEFRAMEWORK_MESSAGECENTER_H

#include <string>
#include <unordered_map>
#include <memory>
#include "Message.h"
#include "ConcurrentRingBuffer.h"
#include "ConcurrentHashMap.h"

using namespace std;

class MessageCenter {
    enum _ErrorType{
        RECEIVER_RECV_OK,
        RECEIVER_NOT_EXIST, //接受者未注册
        RECEIVER_BUSY,      // 接收的队列已满
        UNKNOWN_ERR
    };
    typedef ConcurrentRingBuffer<shared_ptr<Message>> MsgQueue;
    typedef ConcurrentHashMap<string, weak_ptr<MsgQueue>> MsgQueueMap;//每个InteractiveTask独占一个消息队列，生命周期和task一致
public:
    bool registerReceiver(const string& receiver, const weak_ptr<MsgQueue> recevQueue);//注册receiver
    void unregisterReceiver(const string& receiver);                                   //注销
    int  syncSendMsgTo(const string& receiver, const shared_ptr<Message>& message);//同步发送消息
    bool asyncSendMsgTo(const string& receiver, const shared_ptr<Message>& message); //异步发送消息

private:
    MsgQueueMap recevQueueMap_;

};


#endif //SEVICEFRAMEWORK_MESSAGECENTER_H
