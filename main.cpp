#include <iostream>
#include "ThreadPool.h"

int initServer()
{
    google::InitGoogleLogging(LOG_FILE_NAME); //初始化glog库
    FLAGS_stderrthreshold = google::INFO;     //
    FLAGS_log_dir = MA_LOG_DIR;
    FLAGS_max_log_size = 4;
    mkdir("log", 0755);
    signal(SIGPIPE, SIG_IGN);
    google::ShutdownGoogleLogging();
}

int main()
{
    ThreadPool *threadPool = new ThreadPool(1000);
    threadPool->start();
    std::this_thread::sleep_for(std::chrono::seconds(10000));
    return 0;
}