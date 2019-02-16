//
// Created by Shinan on 2019/1/15.
//

#include "ThreadPool.h"
#include <sstream>
#include <logging.h>
#include <assert.h>
using namespace std;

ThreadPool* ThreadPool::pInstance_= nullptr;

ThreadPool *ThreadPool::initInstance(int threadNum, int taskQueueSize)
{
  pInstance_ = new ThreadPool(threadNum, taskQueueSize);
  return pInstance_;
}

ThreadPool *ThreadPool::getInstance()
{
  return pInstance_;
}

ThreadPool::ThreadPool(int threadNum, int taskQueueSize)
    : threadNum_(threadNum), taskQueue_(taskQueueSize)
{}

ThreadPool::~ThreadPool()
{}

void ThreadPool::start()
{
  int threadNum = threadNum_;
  for (; threadNum > 0; threadNum--)
  {
    std::thread threadObj(&ThreadPool::threadEntry, this);
    threadObj.detach();
    vecThreads.push_back(std::move(threadObj));
  }
  LOG(INFO) << vecThreads.size() << " threads has started!";
}

void ThreadPool::syncPostTask(const std::shared_ptr<Task> &task)
{
  // assert(pTask != nullptr);
  taskQueue_.syncPut(task);
}

bool ThreadPool::asyncPostTask(const std::shared_ptr<Task> &task)
{
  //assert(pTask != nullptr);
  return taskQueue_.asyncPut(task);
}

bool ThreadPool::__syncGetOneTask(std::shared_ptr<Task> &task)
{
  //  Task *pTask = nullptr;
  taskQueue_.syncGet(task);
  return true;
}

void ThreadPool::threadEntry()
{
  shared_ptr<Task> spTask;
  __syncGetOneTask(spTask);
  assert(spTask);
  spTask->start();
}



