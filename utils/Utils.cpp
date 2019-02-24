//
// Created by Shinan on 2019/2/20.
//

#include <string>
#include <sstream>
#include <string>
#include <sys/prctl.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include "Utils.h"

namespace Utils
{

bool split(const std::string &str, std::vector<std::string> &vecStr, char delim)
{
  std::istringstream ss(str);
  std::string tmpStr;
  while (std::getline(ss, tmpStr, delim))
    vecStr.emplace_back(std::move(tmpStr));
  return true;
}

//创建event fd
int createEventFd()
{
  //不用设置非阻塞，因为信号量性质，有事件通知一定有事件，不会阻塞。没事件也不会来读
  return eventfd(0, EFD_CLOEXEC | EFD_SEMAPHORE);
}

//设置线程名
bool setThreadName(const std::string &name)
{
  if (name.size() > 15) return false;
  return prctl(PR_SET_NAME, name.c_str(), 0, 0, 0) == 0 ? true : false;
}

}
