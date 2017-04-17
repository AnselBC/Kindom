//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_I_PROTECTEDQUEUE_H
#define TEST_LOCK_I_PROTECTEDQUEUE_H

#include "Kindom.h"
#include "I_Event.h"
#include "kmutex.h"

struct ProtectedQueue {
  void enqueue(Event *e, bool fast_signal = false);
  void signal();
  int try_signal();             // Use non blocking lock and if acquired, signal
  void enqueue_local(Event *e); // Safe when called from the same thread
  void remove(Event *e);
  Event *dequeue_local();
  void dequeue_timed(ktime cur_time, ktime timeout, bool sleep);

  KAtomicList al;
  kmutex lock;
  kcond might_have_data;
  Que(Event, link) localQueue;

  ProtectedQueue();
};

void flush_signals(EThread *t);

#endif // TEST_LOCK_I_PROTECTEDQUEUE_H
