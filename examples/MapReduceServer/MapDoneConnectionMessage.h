//
// Created by Shinan on 2019/2/19.
//

#ifndef SERVICEFRAMEWORK_MAPDONECONNECTIONMESSAGE_H
#define SERVICEFRAMEWORK_MAPDONECONNECTIONMESSAGE_H
#include "network/ConnectionMessage.h"

class MapDoneConnectionMessage : public ConnectionMessage
{
public:
  MapDoneConnectionMessage::MapDoneConnectionMessage(int handle, int maxNum)
      : ConnectionMessage(handle), maxNum_(maxNum)
  {}

  int getMaxNum_() const
  { return maxNum_; }

private:
  int maxNum_;
};

#endif //SERVICEFRAMEWORK_MAPDONECONNECTIONMESSAGE_H
