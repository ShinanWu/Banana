//
// Created by Shinan on 2019/1/15.
//

#ifndef SEVICEFRAMEWORK_TASK_H
#define SEVICEFRAMEWORK_TASK_H

#include <iostream>


using namespace std;

class Task {
    
public:
    Task(const string name);
    virtual void run()=0;

private:
    bool setThreadName(const string name);
    Task();
};

#endif //SEVICEFRAMEWORK_TASK_H
