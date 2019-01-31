//
// Created by Shinan on 2019/1/30.
//

#ifndef SEVICEFRAMEWORK_STREAMPARSER_H
#define SEVICEFRAMEWORK_STREAMPARSER_H

#include <vector>
#include <string>
#include <memory>
#include "Message.h"

class StreamParser
{
  virtual bool recvOnePck();
  virtual bool getPck();
  virtual bool setPck();
  virtual bool sendPck();
};

#endif //SEVICEFRAMEWORK_STREAMPARSER_H
