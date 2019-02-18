//
// Created by Shinan on 2019/1/27.
//

#ifndef SEVICEFRAMEWORK_CONNECTION_H
#define SEVICEFRAMEWORK_CONNECTION_H
#include <memory>
#include <assert.h>
#include "Stream.h"
#include "ConnectionMessage.h"
//前向声明
class NetWorkService;
typedef shared_ptr<NetWorkService> SpNetWorkService;
#include "NetWorkService.h"

class Connection
{
public:
  Connection(const SpStream &spStream, const SpNetWorkService &spNetWorkService);
  virtual ~Connection();

  virtual void startReadOrWriteInService();
  virtual void onMessage(const SpConnectionMessage &spConnectionMessage);
  int getHandle() const;
  void destroy();

public:
  SpStream spStream_;
  SpNetWorkService spNetWorkService_;

private:
  int handle_;

};
typedef shared_ptr<Connection> SpConnection;

#endif //SEVICEFRAMEWORK_CONNECTION_H

