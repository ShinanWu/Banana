//
// Created by Shinan on 2019/1/23.
//

#ifndef SEVICEFRAMEWORK_LIBEVENTOBJ_H
#define SEVICEFRAMEWORK_LIBEVENTOBJ_H


class LibeventObj {
public:
    LibeventObj();
    ~LibeventObj();
    bool init
private:
    struct event_base *base;
    struct evconnlistener *listener;
};


#endif //SEVICEFRAMEWORK_LIBEVENTOBJ_H
