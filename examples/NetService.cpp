//
// Created by Shinan on 2019/1/22.
//创建一个libevent集成的网络服务
//功能是网络通信并生成IndependentTask和InteractiveTask，并和InteractiveTask通信
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <logging.h>
#include "NetService.h"

NetService::NetService(const string &name) : InteractiveTask(name)
{}
NetService::~NetService()
{}
bool NetService::_onStart()
{

}
void NetService::_onStop()
{

}
void NetService::_onMessage(const shared_ptr<Message> &spMessage)
{

}

bool NetService::onStart()
{
  struct event_base *base;
  base = event_base_new();

  m_listener = evconnlistener_new_bind(base, acceptHandler, (void *) m_gbfTh, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                       MAX_LIBEVENT_BACKLOG_COUNT, (struct sockaddr *) &sin, sizeof(sin));

  event_base_dispatch(base);//while(1) �����¼�
  event_base_free(base);
  return false;
}

bool NetService::onStop()
{
  return false;
}

void NetService::onMessage(const shared_ptr<Message> &spMessage)
{

}

void NetService::acceptHandler(struct evconnlistener *listener,
                               evutil_socket_t cfd,
                               struct sockaddr *addr,
                               int len,
                               void *ptr)
{
  GBF_PARM *pParm = static_cast<GBF_PARM *>(ptr);

  LOG(INFO) << "accept client ip:" << inet_ntoa(((struct sockaddr_in *) addr)->sin_addr) << "  prot:"
            << ((struct sockaddr_in *) addr)->sin_port << " fd:" << cfd;
  struct timeval tv = {GBF_CONNECT_TIMEOUT, 0};
  struct event_base *pbase = evconnlistener_get_base(listener);
  struct event *pEvread = (event *) malloc(sizeof(struct event));
  //---------------------------------------------------------------------------------
  ST_THREAD_PAR *pPar = new ST_THREAD_PAR(pParm->pThis);
  //pPar->pThreadPar = pPar;
  pPar->setFd(cfd);
  //pPar->pEvent = pEvread;
  pPar->setEvent(pEvread);
  //----------------------------------------------------------------------------------
  event_set(pEvread, cfd, EV_READ | EV_PERSIST | EV_TIMEOUT, on_read_callback, (void *) pPar);
  event_base_set(pbase, pEvread);
  event_add(pEvread, &tv);
}

void NetService::readHandler(evutil_socket_t fd, short events, void *arg)
{

}

