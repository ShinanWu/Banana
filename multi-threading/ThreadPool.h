//
// Created by Shinan on 2019/1/15.
//

#ifndef SEVICEFRAMEWORK_THREADPOOL_H
#define SEVICEFRAMEWORK_THREADPOOL_H

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <memory>
#include "Task.h"
#include "ConcurrentRingBuffer.h"

#define TASK_QUEUE_MAX_SIZE 1000

class ThreadPool
{
  //只有TcpServer可以初始化线程池
  friend class TcpServer;
private:
  int threadNum_;
  std::vector<std::thread> vecThreads;
  ConcurrentRingBuffer<std::shared_ptr<Task>> taskQueue_;

public:
  static ThreadPool *getInstance();
  ~ThreadPool();
  void syncPostTask(const std::shared_ptr<Task> &task); //线程安全
  bool asyncPostTask(const std::shared_ptr<Task> &task);
  void start();
private:
  static ThreadPool *initInstance(int threadNum, int taskQueueSize = TASK_QUEUE_MAX_SIZE);
  static ThreadPool *pInstance_;
  explicit ThreadPool(int threadNum, int taskQueueSize = TASK_QUEUE_MAX_SIZE);
  bool __syncGetOneTask(std::shared_ptr<Task> &task);
  void threadEntry();
};

#endif //SEVICEFRAMEWORK_THREADPOOL_H
