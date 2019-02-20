#include "logging.h"
#include "Task.h"

class IndependentTask : public Task
{
public:
  IndependentTask(const string &name);
  virtual ~IndependentTask();
  virtual void start() final;
  virtual void run() = 0;//子类只需要实现run函数
};