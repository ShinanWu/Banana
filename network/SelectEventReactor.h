//
// Created by Shinan on 2019/2/1.
//
//此reactor不做密集网络IO，而是做处理事件少量网络IO和其他耗时任务，比如同步查询和处理
//数据库操作，或者做录像存储，视频转发等，这里的网络IO多是控制命令，如下载进度查询，播放控制，
//播放的seek操作等。

#ifndef SEVICEFRAMEWORK_SELECTEVENTREACTOR_H
#define SEVICEFRAMEWORK_SELECTEVENTREACTOR_H
#include "EventReactor.h"

class SelectEventReactor : public EventReactor
{
public:
  virtual bool createReactor(int maxFds);
  virtual void destroyReactor();
  virtual bool addEventHandler(int fd, short event, const EventCallback &cb);
  virtual bool enableEvent(int fd, short events);
  virtual bool disableEvent(int fd, short events);
  virtual bool removeEventHandler(int fd, short event, const EventCallback &cb);
  virtual void startEventLoop();
};

#endif //SEVICEFRAMEWORK_SELECTEVENTREACTOR_H
