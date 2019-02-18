//
// Created by Shinan on 2019/1/20.
//
#include <logging.h>
#include <assert.h>
#include "IndependentTask.h"

IndependentTask::IndependentTask(const string &name) : Task(name)
{}
IndependentTask::~IndependentTask()
{}

void IndependentTask::start()
{
  LOG(INFO) << "Task " << getTaskName() << " started!";
  run();
  LOG(INFO) << "Task " << getTaskName() << " finished!";
}


