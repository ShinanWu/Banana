//
// Created by Shinan on 2019/1/15.
//

#ifndef SEVICEFRAMEWORK_TASK_H
#define SEVICEFRAMEWORK_TASK_H

#include <iostream>
#include <string>
#include "logging.h"

using namespace std;

class Task
{
public:
  Task(const string &name):taskName_(name)
  {
  //  DLOG(INFO) << "Task " << name << " created!";
  }
  virtual ~Task()
  {
   // LOG(INFO) << "Task " << taskName_ << " destroyed!";
  }
  virtual void start() = 0;

  const string &getTaskName() const
  {
    return taskName_;
  }
private:
  const string taskName_;
};


#endif //SEVICEFRAMEWORK_TASK_H
