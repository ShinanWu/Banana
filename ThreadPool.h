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

class ThreadPool {

private:
    int threadNum_;
    std::vector<std::thread> vecThreads;
    ConcurrentRingBuffer<std::shared_ptr<Task>> taskQueue_;

public:
    explicit ThreadPool(int threadNum, int taskQueueSize=TASK_QUEUE_MAX_SIZE);
    ~ThreadPool();
    void syncPostTask(const std::shared_ptr<Task> &task);
    bool asyncPostTask(const std::shared_ptr<Task> &task);
    void start();
private:
    ThreadPool();
    bool __syncGetOneTask(std::shared_ptr<Task> &task);
    void threadEntry();
};


#endif //SEVICEFRAMEWORK_THREADPOOL_H
