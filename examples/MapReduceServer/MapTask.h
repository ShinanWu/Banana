//
// Created by Shinan on 2019/2/18.
//

#ifndef SERVICEFRAMEWORK_MAPTASK_H
#define SERVICEFRAMEWORK_MAPTASK_H

#endif //SERVICEFRAMEWORK_MAPTASK_H
#include "multi-threading/IndependentTask.h"

template<class Ret, class Ele>
vector<Ret> map(function<Ret(Ele)> fun, vector<Ele> vec)
{
  vector<Ret> vecRet;
  for (auto it : vec)
  {
    vecRet.emplace_back(fun(it));
  }
  return vecRet;
}

class MapTask : public IndependentTask
{
  typedef function<void(void)> RunCallback;
public:
  MapTask(const RunCallback &runCallback) : IndependentTask("map"), runCallback_(runCallback)
  {}

  virtual void run()
  {
    runCallback_();
    runCallback_ = nullptr;
  }

private:
  RunCallback runCallback_;
};