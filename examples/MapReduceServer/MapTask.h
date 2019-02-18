//
// Created by Shinan on 2019/2/18.
//

#ifndef SERVICEFRAMEWORK_MAPTASK_H
#define SERVICEFRAMEWORK_MAPTASK_H

#endif //SERVICEFRAMEWORK_MAPTASK_H
#include "multi-threading/IndependentTask.h"

class MapTask : public IndependentTask
{
  typedef function<void()> RunCallback;
public:
  MapTask(const RunCallback &runCallback_) : runCallback_(runCallback_)
  {}

  virtual void run()
  {
    runCallback_();
  }

private:
  RunCallback runCallback_;
};