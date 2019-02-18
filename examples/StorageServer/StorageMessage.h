//
// Created by Shinan on 2019/2/11.
//

#ifndef SEVICEFRAMEWORK_STORAGEMESSAGE_H
#define SEVICEFRAMEWORK_STORAGEMESSAGE_H

#include "multi-threading/Message.h"
#include <string>
using namespace std;

class StorageMessage : Message
{
public:
  const string &getCmd_() const
  {
    return cmd_;
  }
  void setCmd_(const string &cmd)
  {
    cmd_ = cmd;
  }

private:
  string cmd_;
};

#endif //SEVICEFRAMEWORK_STORAGEMESSAGE_H
