//
// Created by Shinan on 2019/2/20.
//

#include <string>
#include <sstream>
#include "Utils.h"
using namespace Utils;

bool ::split(const std::string &str, std::vector<std::string> &vecStr, char delim)
{
  std::istringstream ss(str);
  std::string tmpStr;
  while (std::getline(ss, tmpStr, delim))
    vecStr.emplace_back(std::move(tmpStr));
  return true;
}