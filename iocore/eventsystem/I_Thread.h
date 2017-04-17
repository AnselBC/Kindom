//
// Created by 宋辰伟 on 2017/4/16.
//

#ifndef PROJECT_I_THREAD_H
#define PROJECT_I_THREAD_H

#include <memory>
#include "kthread.h"
#include "khrtime.h"

class Mutex;
class Thread;

typedef void *(*ThreadFunction)(void *arg);

static const int DEFAULT_STACKSIZE = 1048576; // 1MB

class Thread
{
public:
  // Thread mutex;
  std::shared_ptr<Mutex> mutex;
  // Thread tid;
  kthread tid;
  // Thread specified data.
  static kthread_key thread_data_key;

  // Time
  static khrtime cur_time;

  static void update_time(){};

  Thread();

  static khrtime
  get_hrtime()
  {
    return cur_time;
  }

  void
  set_specific();

  const char *
  get_thread_name()
  {
    return thr_name;
  }

  void
  set_thread_name(const char *name)
  {
    thr_name = name;
  }

  kthread start(const char *name, size_t stacksize, ThreadFunction f, void *a, void *stack);

  virtual void
  execute()
  {
  }

  static khrtime get_hrtime_updated();

	~Thread();

private:
  const char *thr_name;
};

extern Thread *this_thread();

inline khrtime
Thread::get_hrtime_updated()
{
  return cur_time = kget_hrtime_internal();
}

#endif // PROJECT_I_THREAD_H
