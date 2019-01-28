//
// Created by Shinan on 2019/1/22.
//

#ifndef SEVICEFRAMEWORK_NETSERVICE_H
#define SEVICEFRAMEWORK_NETSERVICE_H


#include <InteractiveTask.h>

class NetService : public InteractiveTask{
public:
    NetService(const string &name);

    bool init() override;

    void onMessage(const shared_ptr<Message> &spMessage) override;
    static void acceptHandler(struct evconnlistener *listener, evutil_socket_t cfd, struct sockaddr* addr, int len, void *ptr);
    static void readHandler(evutil_socket_t fd, short events, void* arg);

};


#endif //SEVICEFRAMEWORK_NETSERVICE_H
