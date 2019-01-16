#include <iostream>
#include "ThreadPool.h"


int main() {
    ThreadPool *threadPool = new ThreadPool(1000);
    threadPool->start();
    std::this_thread::sleep_for(std::chrono::seconds(10000));
    return 0;
}