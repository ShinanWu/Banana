//
// Created by Shinan on 2019/1/19.
//

#include "Message.h"


Message::Message(const string &what_, const MessageCallBack &messageCallBack_)
    : what_(what_)
    , messageCallBack_(std::move(messageCallBack_))//移动构造提高效率
{}
bool Message::getWhat_(string &what) const
{
    what = std::move(what_);
    return true;
}
bool Message::getMsgCallBack(Message::MessageCallBack &msgCallBack) const
{
    msgCallBack = std::move(messageCallBack_);
    return true;
}
