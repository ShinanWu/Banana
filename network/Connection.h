//
// Created by Shinan on 2019/1/27.
//

#ifndef SEVICEFRAMEWORK_CONNECTION_H
#define SEVICEFRAMEWORK_CONNECTION_H
#include <memory>
#include <network/Stream.h>
#include "Stream.h"
#include "network/EventReactor.h"

class Connection
{
public:
  Connection(const SpStream& spStream);
  void destroy();

private:
  virtual ~Connection();

private:
  SpStream spStream_;
};

#endif //SEVICEFRAMEWORK_CONNECTION_H

