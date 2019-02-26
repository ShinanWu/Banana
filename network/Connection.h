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
typedef weak_ptr<NetWorkService> WpNetWorkService;
typedef shared_ptr<NetWorkService> SpNetWorkService;

class Connection
{
public:
  explicit Connection(const SpStream &spStream);
  Connection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService);
  virtual ~Connection();

  virtual void startReadOrWriteInService();
  virtual void onMessage(const SpConnectionMessage &spConnectionMessage);
  SpNetWorkService getSpNetWorkService();
  int getHandle() const;
  void destroy();

public:
  SpStream spStream_;
  WpNetWorkService wpNetWorkService_;

private:
  int handle_;

};
typedef shared_ptr<Connection> SpConnection;

#endif //SEVICEFRAMEWORK_CONNECTION_H

