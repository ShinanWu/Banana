//
// Created by Shinan on 2019/1/27.
//

#ifndef SEVICEFRAMEWORK_CONNECTION_H
#define SEVICEFRAMEWORK_CONNECTION_H
#include <memory>
#include <network/Stream.h>
//#include "Stream.h"
#include "network/EventReactor.h"

class Connection
{
public:
  virtual ~Connection();
  Connection();
};

#endif //SEVICEFRAMEWORK_CONNECTION_H

