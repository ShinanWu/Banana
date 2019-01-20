//
// Created by Shinan on 2019/1/9.
//
#include <logging.h>
#include "MessageCenter.h"

bool MessageCenter::registerReceiver(const string &receiver, const weak_ptr<MessageCenter::MsgQueue> recevQueue) {
    return recevQueueMap_.insert(receiver, recevQueue);
}

void MessageCenter::unregisterReceiver(const string &receiver) {
    recevQueueMap_.erase(receiver);
}

int MessageCenter::syncSendMsgTo(const string &receiver, const shared_ptr<Message> &message) {
    weak_ptr<MessageCenter::MsgQueue> wpMsgQueue;

    if(!recevQueueMap_.find(receiver, wpMsgQueue)){
        LOG(ERROR) << receiver << " not registered";
        return MessageCenter::RECEIVER_NOT_EXIST; //未注册
    }
    shared_ptr spMsgQueue = wpMsgQueue.lock(); //线程安全的类型提升
    if(spMsgQueue == nullptr){
        LOG(ERROR) << receiver << " unregistered!";
        return MessageCenter::RECEIVER_NOT_EXIST;
    }
    //注意share_ptr赋值的时候之前的指针处理方式
    if(spMsgQueue->syncPut(message))
        return true;
    return MessageCenter::UNKNOWN_ERR;
}

bool MessageCenter::asyncSendMsgTo(const string &receiver, const shared_ptr<Message> &message) {
    //to-do,暂未实现
    return false;
}
