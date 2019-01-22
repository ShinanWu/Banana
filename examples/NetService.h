//
// Created by Shinan on 2019/1/22.
//

#ifndef SEVICEFRAMEWORK_NETSERVICE_H
#define SEVICEFRAMEWORK_NETSERVICE_H


#include <InteractiveTask.h>

class NetService : public InteractiveTask{
public:
    NetService(string name);

    NetService(const string &name);

    bool init() override;

    void onMessage(const shared_ptr<Message> &spMessage) override;

};


#endif //SEVICEFRAMEWORK_NETSERVICE_H
