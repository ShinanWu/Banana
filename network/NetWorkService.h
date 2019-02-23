//
// Created by Shinan on 2019/2/14.
//

#ifndef SERVICEFRAMEWORK_NETWORKERSERVICE_H
#define SERVICEFRAMEWORK_NETWORKERSERVICE_H


#include <functional>
#include <multi-threading/InteractiveTask.h>
#include <unordered_map>
#include "Stream.h"
class Connection;
typedef std::shared_ptr<Connection> SpConnection;
#include "Connection.h"
#include "multi-threading/WaitFreeConcurrentRingBuffer.h"
#include "NewConnectMessage.h"
using namespace std;

class NetWorkService : public InteractiveTask
{
  typedef WaitFreeConcurrentRingBuffer<NewConnectMessage> NewConMsgWaitFreeBuf;
  typedef function<void(const SpStream&, const shared_ptr<NetWorkService>&)> ConnectionCallback;
public:
  NetWorkService(const string &name, const SpEventReactor &spEventReactor);
  virtual ~NetWorkService();
  void setNewConnectionCallback(const ConnectionCallback &cb);
  bool addNewConnection(const SpConnection& spConnection);
  void removeConnection(int handle);
  bool notifyNewConnnectMsg(const NewConnectMessage &newConMsg);


private:
  virtual void onStart() final;
  virtual void onStop() final;
  virtual void onMessage(const shared_ptr<Message> &spMessage) final;
  void onNewConMsg(int fd, short event);

private:
  NewConMsgWaitFreeBuf newConMsgBuf_{500};
  int acceptEventFd_{-1};
  ConnectionCallback newConnectionCb_;
  EventReactor::EventCallback onNewConMsgCall_;
  unordered_map<int, SpConnection> connectionMap_;
};
typedef shared_ptr<NetWorkService> SpNetWorkService;
typedef weak_ptr<NetWorkService> WpNetWorkService;

#endif //SERVICEFRAMEWORK_NETWORKERSERVICE_H
