//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_I_ACTION_H
#define TEST_LOCK_I_ACTION_H

#include "I_Thread.h"
#include "I_Continuation.h"

class Action
{
public:
  Continuation *continuation;

  std::shared_ptr<Mutex> mutex;

  volatile int cancelled;

  virtual void
  cancel(Continuation *c = nullptr)
  {
    kassert(!c || c == continuation);
#ifdef DEBUG
    kassert(!cancelled);
    cancelled = true;
#else
    if (!cancelled)
      cancelled = true;
#endif
  }

  void
  cancel_action(Continuation *c = nullptr)
  {
    kassert(!c || c == continuation);
#ifdef DEBUG
    kassert(!cancelled);
    cancelled = true;
#else
    if (!cancelled)
      cancelled = true;
#endif
  }

  Continuation *
  operator=(Continuation *acont)
  {
    continuation = acont;
    if (acont)
      mutex = acont->mutex;
    else
      mutex = 0;
    return acont;
  }

  Action() : continuation(nullptr), cancelled(false) {}
  virtual ~Action() { mutex = nullptr; }
};

#define ACTION_RESULT_NONE MAKE_ACTION_RESULT(0)
#define ACTION_RESULT_DONE MAKE_ACTION_RESULT(1)
#define ACTION_IO_ERROR MAKE_ACTION_RESULT(2)
#define ACTION_RESULT_INLINE MAKE_ACTION_RESULT(3)

// Use these classes by
// #define ACTION_RESULT_HOST_DB_OFFLINE
//   MAKE_ACTION_RESULT(ACTION_RESULT_HOST_DB_BASE + 0)

#define MAKE_ACTION_RESULT(_x) (Action *)(((uintptr_t)((_x << 1) + 1)))

#endif // TEST_LOCK_I_ACTION_H
