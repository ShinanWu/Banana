//
// Created by Shinan on 2019/2/24.
//

#ifndef SERVICEFRAMEWORK_CONCURRENCEECHOSERVER_H
#define SERVICEFRAMEWORK_CONCURRENCEECHOSERVER_H

#include <network/TcpServer.h>
class ConcurrenceEchoServer : public TcpServer
{
public:
  ConcurrenceEchoServer(int netWorkServiceNum, unsigned short listenPort, int threadPoolNum);

protected:
  virtual void onConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService);
};

#endif //SERVICEFRAMEWORK_CONCURRENCEECHOSERVER_H
