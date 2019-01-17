//
// Created by Shinan on 2019/1/9.
//

#ifndef SEVICEFRAMEWORK_SERVICE_H
#define SEVICEFRAMEWORK_SERVICE_H

#include "Task.h"
#include <logging.h>

class Service : Task{
private:
    bool setThreadName(const string name);
};


#endif //SEVICEFRAMEWORK_SERVICE_H
