//
// Created by Shinan on 2019/1/22.
//创建一个libevent集成的网络服务

#include "NetService.h"

NetService::NetService(const string &name) : InteractiveTask(name) {}

bool NetService::init() {

    return false;
}

void NetService::onMessage(const shared_ptr<Message> &spMessage) {

}
