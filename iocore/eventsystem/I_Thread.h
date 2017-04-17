//
// Created by 宋辰伟 on 2017/4/16.
//

#ifndef PROJECT_I_THREAD_H
#define PROJECT_I_THREAD_H

#include <memory>
#include "kthread.h"

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
  static ktime cur_time;

  static void update_time(){};

  Thread();

  static ktime
  get_time()
  {
    return cur_time;
  }

  void
  set_specific()
  {
    assert(!pthread_setspecific(thread_data_key, this));
  }

  Thread *
  this_thread()
  {
    return (Thread *)pthread_getspecific(thread_data_key);
  }

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

private:
  const char *thr_name;
};

#endif // PROJECT_I_THREAD_H
