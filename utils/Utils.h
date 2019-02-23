//
// Created by Shinan on 2019/2/20.
//

#ifndef SERVICEFRAMEWORK_UTILS_H
#define SERVICEFRAMEWORK_UTILS_H
#include <string>
#include <vector>
#include <sstream>
#include <sys/eventfd.h>
#include <unistd.h>

namespace Utils
{

bool split(const std::string &str, std::vector<std::string> &vecStr, char delim);

int createEventFd();

bool setThreadName(const std::string &name);


}

#endif //SERVICEFRAMEWORK_UTILS_H
