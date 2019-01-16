//
// Created by Shinan on 2019/1/15.
//

#ifndef SEVICEFRAMEWORK_THREADPOOL_H
#define SEVICEFRAMEWORK_THREADPOOL_H

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include "Task.h"
#include "ConcurrentRingBuffer.h"

#define TASK_QUEUE_MAX_SIZE 1000

class ThreadPool {

private:
    int threadNum_;
    std::vector<std::thread> vecThreads;
    ConcurrentRingBuffer<Task*> taskQueue_;

public:
    explicit ThreadPool(int threadNum, int taskNum=TASK_QUEUE_MAX_SIZE);
    ~ThreadPool();
    void syncPostTask(Task* task);
    bool asyncPostTask(Task* task);
    void start();
private:
    ThreadPool();
    Task* __syncGetOneTask();
    void threadEntry();
};


#endif //SEVICEFRAMEWORK_THREADPOOL_H
