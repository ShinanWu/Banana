//
// Created by Shinan on 2019/2/20.
//

#ifndef SERVICEFRAMEWORK_UTILS_H
#define SERVICEFRAMEWORK_UTILS_H
#include <string>
#include <vector>
#include <sstream>

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

}

#endif //SERVICEFRAMEWORK_UTILS_H
