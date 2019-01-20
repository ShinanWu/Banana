//
// Created by Shinan on 2019/1/19.
//
#include "InteractiveTask.h"
#include <sys/prctl.h>


InteractiveTask::InteractiveTask(const string &name):taskName_(name) {

}

const string &InteractiveTask::getTaskName_() const {
    return taskName_;
}

void InteractiveTask::setTaskName_(const string &taskName_) {
    InteractiveTask::taskName_ = taskName_;
}

bool InteractiveTask::_setThreadName(const string &name) {
    if(prctl(PR_SET_NAME, name.c_str(), 0, 0, 0) == -1)
    {
        LOG(ERROR) << "set thread name failed! name:" << name;
        return false;
    }
    return true;
}

void InteractiveTask::start() {
    _setThreadName(taskName_);
}

