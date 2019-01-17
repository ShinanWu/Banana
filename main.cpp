#include <iostream>
#include "ThreadPool.h"
#include <logging.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int initLog()
{
    google::InitGoogleLogging("ServiceFramework.log"); //初始化glog库
    FLAGS_stderrthreshold = google::INFO;     //
    mkdir("log", 0755);
    FLAGS_log_dir = "./log";
    FLAGS_max_log_size = 4;
}

int main()
{
    signal(SIGPIPE, SIG_IGN);
    initLog();
    ThreadPool *threadPool = new ThreadPool(1000);
    threadPool->start();
    std::this_thread::sleep_for(std::chrono::seconds(10000));
    google::ShutdownGoogleLogging();
    return 0;
}