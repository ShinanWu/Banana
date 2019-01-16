//
// Created by Shinan on 2019/1/15.
//

#include "ThreadPool.h"
#include <sstream>

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

void ThreadPool::syncPostTask(Task* pTask) {
    assert(pTask != nullptr);
    taskQueue_.syncPut(pTask);
}

bool ThreadPool::asyncPostTask(Task* pTask) {
    assert(pTask != nullptr);
    return taskQueue_.asyncPut(pTask);
}
Task *ThreadPool::__syncGetOneTask() {
    Task *pTask = nullptr;
    taskQueue_.syncGet(pTask);
    return pTask;
}

void ThreadPool::threadEntry() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    printf("threadId:%s has started\n", ss.str().c_str());
}

