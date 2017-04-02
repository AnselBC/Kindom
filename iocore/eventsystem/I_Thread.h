//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef KINDOM_I_THREAD_H
#define KINDOM_I_THREAD_H

#include "king/king_platform.h"
#include "king/king_thread.h"
#include "king/king_memory.h"

class Thread;
// class ProxyMutex;
typedef void *(*ThreadFunction)(void *arg);

static const int MAX_THREAD_NAME_LENGTH = 16;
static const int DEFAULT_STACKSIZE      = 1048576; // 1MB

class Thread
{
public:
  /*-------------------------------------------*\
  | Common Interface                            |
  \*-------------------------------------------*/

  /**
    System-wide thread identifier. The thread identifier is represented
    by the platform independent type ink_thread and it is the system-wide
    value assigned to each thread. It is exposed as a convenience for
    processors and you should not modify it directly.

  */
  kthread tid;

  /**
    Thread lock to ensure atomic operations. The thread lock available
    to derived classes to ensure atomic operations and protect critical
    regions. Do not modify this member directly.

  */
  //    Ptr<ProxyMutex> mutex;

  // PRIVATE
  void set_specific();
  Thread();
  //    virtual ~Thread();

  //    static ink_hrtime cur_time;
  static kthread_key thread_data_key;

private:
  // prevent unauthorized copies (Not implemented)
  Thread(const Thread &);
  Thread &operator=(const Thread &);

public:
  kthread start(const char *name, size_t stacksize, ThreadFunction f, void *a, void *stack);

  virtual void
  execute()
  {
  }

  /** Get the current ATS high resolution time.
      This gets a cached copy of the time so it is very fast and reasonably accurate.
      The cached time is updated every time the actual operating system time is fetched which is
      at least every 10ms and generally more frequently.
      @note The cached copy shared among threads which means the cached copy is udpated
      for all threads if any thread updates it.
  */
  //    static ink_hrtime get_hrtime();

  /** Get the operating system high resolution time.

      Get the current time at high resolution from the operating system.  This is more expensive
      than @c get_hrtime and should be used only where very precise timing is required.

      @note This also updates the cached time.
  */
  //    static ink_hrtime get_hrtime_updated();
};

// extern Thread *this_thread();

#endif // KINDOM_I_THREAD_H
