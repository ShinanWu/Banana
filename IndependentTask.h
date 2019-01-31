#include "glog.h"
#include "Task.h"

class IndependentTask : public Task{
public:
    virtual void start() final;
    virtual bool init();
    virtual void run() = 0;//子类只需要实现start函数

};