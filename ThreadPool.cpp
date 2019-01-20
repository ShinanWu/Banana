//
// Created by Shinan on 2019/1/15.
//

#include "ThreadPool.h"
#include <sstream>
#include <logging.h>
#include <assert.h>

ThreadPool::ThreadPool(int threadNum, int taskNum)
    :threadNum_(threadNum)
    ,taskQueue_(taskNum)
{

}

ThreadPool::~ThreadPool() {

}

void ThreadPool::start() {
    int threadNum = threadNum_;
    for(; threadNum > 0; threadNum--){
        std::thread threadObj(&ThreadPool::threadEntry, this);
        threadObj.detach();
        vecThreads.push_back(std::move(threadObj));
    }
    std::cout << vecThreads.size() << " threads has started" << std::endl;
}

void ThreadPool::syncPostTask(const std::shared_ptr<Task> &task) {
   // assert(pTask != nullptr);
    taskQueue_.syncPut(task);
}

bool ThreadPool::asyncPostTask(const std::shared_ptr<Task> &task) {
    //assert(pTask != nullptr);
    return taskQueue_.asyncPut(task);
}

bool ThreadPool::__syncGetOneTask(std::shared_ptr<Task> &task) {
  //  Task *pTask = nullptr;
    taskQueue_.syncGet(task);
    return true;
}

void ThreadPool::threadEntry() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    LOG(INFO) << "threadId:" << ss.str() << " has started";
}

