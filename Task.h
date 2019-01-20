//
// Created by Shinan on 2019/1/15.
//

#ifndef SEVICEFRAMEWORK_TASK_H
#define SEVICEFRAMEWORK_TASK_H

#include <iostream>


using namespace std;

class Task {
    
public:
    virtual void start();
    virtual void run();
    virtual void onMessage();

};

#endif //SEVICEFRAMEWORK_TASK_H
