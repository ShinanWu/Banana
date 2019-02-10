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
  Task(const string &name);
  virtual ~Task();
  virtual void start() = 0;
  const string &Task::getTaskName() const;
private:
  const string taskName_;
};

Task::Task(const string &name) : taskName_(name)
{
  LOG(INFO) << "Task " << taskName_ << " created!";
}

Task::~Task()
{
  LOG(INFO) << "Task " << taskName_ << " destroyed!";
}

const string &Task::getTaskName() const
{
  return taskName_;
}

#endif //SEVICEFRAMEWORK_TASK_H
