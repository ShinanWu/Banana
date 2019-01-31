//
// Created by Shinan on 2019/1/20.
//
#include <logging.h>
#include "IndependentTask.h"

void IndependentTask::start()
{
  if (init())
  {
    run();
  }
  DLOG(ERROR) << "task finished!";
}
bool IndependentTask::init()
{
  return true;
}
