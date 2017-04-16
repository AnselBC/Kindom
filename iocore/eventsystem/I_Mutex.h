//
// Created by 宋辰伟 on 2017/4/16.
//

#ifndef PROJECT_I_MUTEX_H
#define PROJECT_I_MUTEX_H

#include "EventSystem.h"
#include "SourceLocation.h"

#ifdef DEBUG
#define SCOPED_MUTEX_LOCK(_l, _m, _t) MutexLock _l(MakeSourceLocation(), nullptr, _m, _t)
#else
#define SCOPED_MUTEX_LOCK(_l, _m, _t) MutexLock _l(_m, _t)
#endif // DEBUG

#ifdef DEBUG
/**
  Attempts to acquire the lock to the ProxyMutex.

  This macro attempts to acquire the lock to the specified ProxyMutex
  object in a non-blocking manner. After using the macro you can
  see if it was successful by comparing the lock variable with true
  or false (the variable name passed in the _l parameter).

  @param _l Arbitrary name for the lock to use in this call (lock variable)
  @param _m A pointer to (or address of) a ProxyMutex object
  @param _t The current EThread executing your code.

*/
#define MUTEX_TRY_LOCK(_l, _m, _t) MutexTryLock _l(MakeSourceLocation(), (char *)nullptr, _m, _t)

/**
  Attempts to acquire the lock to the ProxyMutex.

  This macro performs up to the specified number of attempts to
  acquire the lock on the ProxyMutex object. It does so by running
  a busy loop (busy wait) '_sc' times. You should use it with care
  since it blocks the thread during that time and wastes CPU time.

  @param _l Arbitrary name for the lock to use in this call (lock variable)
  @param _m A pointer to (or address of) a ProxyMutex object
  @param _t The current EThread executing your code.
  @param _sc The number of attempts or spin count. It must be a positive value.

*/
#define MUTEX_TRY_LOCK_SPIN(_l, _m, _t, _sc) MutexTryLock _l(MakeSourceLocation(), (char *)nullptr, _m, _t, _sc)

/**
  Attempts to acquire the lock to the ProxyMutex.

  This macro attempts to acquire the lock to the specified ProxyMutex
  object in a non-blocking manner. After using the macro you can
  see if it was successful by comparing the lock variable with true
  or false (the variable name passed in the _l parameter).

  @param _l Arbitrary name for the lock to use in this call (lock variable)
  @param _m A pointer to (or address of) a ProxyMutex object
  @param _t The current EThread executing your code.
  @param _c Continuation whose mutex will be attempted to lock.

*/

#define MUTEX_TRY_LOCK_FOR(_l, _m, _t, _c) MutexTryLock _l(MakeSourceLocation(), nullptr, _m, _t)
#else // DEBUG
#define MUTEX_TRY_LOCK(_l, _m, _t) MutexTryLock _l(_m, _t)
#define MUTEX_TRY_LOCK_SPIN(_l, _m, _t, _sc) MutexTryLock _l(_m, _t, _sc)
#define MUTEX_TRY_LOCK_FOR(_l, _m, _t, _c) MutexTryLock _l(_m, _t)
#endif // DEBUG

#define MUTEX_RELEASE(_l) (_l).release()

/////////////////////////////////////
// DEPRECATED DEPRECATED DEPRECATED
#ifdef DEBUG
#define MUTEX_TAKE_TRY_LOCK(_m, _t) Mutex_trylock(MakeSourceLocation(), (char *)nullptr, _m, _t)
#define MUTEX_TAKE_TRY_LOCK_FOR(_m, _t, _c) Mutex_trylock(MakeSourceLocation(), (char *)nullptr, _m, _t)
#define MUTEX_TAKE_TRY_LOCK_FOR_SPIN(_m, _t, _c, _sc) Mutex_trylock_spin(MakeSourceLocation(), nullptr, _m, _t, _sc)
#else
#define MUTEX_TAKE_TRY_LOCK(_m, _t) Mutex_trylock(_m, _t)
#define MUTEX_TAKE_TRY_LOCK_FOR(_m, _t, _c) Mutex_trylock(_m, _t)
#define MUTEX_TAKE_TRY_LOCK_FOR_SPIN(_m, _t, _c, _sc) Mutex_trylock_spin(_m, _t, _sc)
#endif

#ifdef DEBUG
#define MUTEX_TAKE_LOCK(_m, _t) Mutex_lock(MakeSourceLocation(), (char *)nullptr, _m, _t)
#define MUTEX_TAKE_LOCK_FOR(_m, _t, _c) Mutex_lock(MakeSourceLocation(), nullptr, _m, _t)
#else
#define MUTEX_TAKE_LOCK(_m, _t) Mutex_lock(_m, _t)
#define MUTEX_TAKE_LOCK_FOR(_m, _t, _c) Mutex_lock(_m, _t)
#endif // DEBUG

#define MUTEX_UNTAKE_LOCK(_m, _t) Mutex_unlock(_m, _t)

typedef pthread_mutex_t kmutex;

class Thread;
typedef Thread *ThreadPtr;

class x_pthread_mutexattr_t
{
public:
  pthread_mutexattr_t attr;
  x_pthread_mutexattr_t();
  ~x_pthread_mutexattr_t() {}
};

inline x_pthread_mutexattr_t::x_pthread_mutexattr_t()
{
  pthread_mutexattr_init(&attr);
#ifndef POSIX_THREAD_10031c
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
#endif
}

static x_pthread_mutexattr_t _g_mattr;

class Mutex
{
public:
  kmutex mutex;
  volatile ThreadPtr thread_holding;
  int nthread_holding;

#ifdef DEBUG
  ktime hold_time;
  SourceLocation srcloc;
  const char *handler;
  int taken;
#endif
  ~Mutex() { pthread_mutex_destroy(&mutex); }
  Mutex()
#ifdef DEBUG
    : srcloc(nullptr, nullptr, 0)
#endif
  {
    thread_holding  = nullptr;
    nthread_holding = 0;
#ifdef DEBUG
    taken     = 0;
    handler   = nullptr;
    hold_time = 0;
#endif
  }

  void
  init(const char *name = "UnnamedMutex")
  {
    if (pthread_mutex_init(&mutex, &_g_mattr.attr) != 0) {
      abort();
    }
  }
};

inline bool
Mutex_trylock(
#ifdef DEBUG
  const SourceLocation &location, const char *ahandler,
#endif
  Mutex *m, Thread *t)
{
  kassert(t != nullptr);
  kassert(m != nullptr);
  kassert(t == t->this_thread());
  if (m->thread_holding != t) {
    if (pthread_mutex_trylock(&m->mutex)) {
      return false;
    }
    m->thread_holding = t;
#ifdef DEBUG
    m->srcloc  = location;
    m->handler = ahandler;
    m->taken++;
#endif
  }
  m->nthread_holding++;
  return true;
}

inline bool
Mutex_trylock_spin(
#ifdef DEBUG
  const SourceLocation &location, const char *ahandler,
#endif
  Mutex *m, Thread *t, int spincnt = 1)
{
  kassert(t != nullptr);
  kassert(m != nullptr);
  kassert(t == t->this_thread());
  if (m->thread_holding != t) {
    int locked;
    do {
      if (locked = (pthread_mutex_trylock(&m->mutex) == 0))
        break;
    } while (--spincnt);
    if (!locked) {
      return false;
    }
    m->thread_holding = t;
#ifdef DEBUG
    m->srcloc  = location;
    m->handler = ahandler;
    m->taken++;
#endif
  }
  m->nthread_holding++;
  return true;
}

inline int
Mutex_lock(
#ifdef DEBUG
  const SourceLocation &location, const char *ahandler,
#endif
  Mutex *m, Thread *t)
{
  kassert(t != nullptr);
  kassert(m != nullptr);
  kassert(t == t->this_thread());
  if (m->thread_holding != t) {
    if (pthread_mutex_lock(&m->mutex) != 0) {
      Fatal("Pthread lock acquire error");
    }
    m->thread_holding = t;
#ifdef DEBUG
    m->srcloc  = location;
    m->handler = ahandler;
    m->taken++;
#endif
  }
  m->nthread_holding++;
  return true;
}

inline void
Mutex_unlock(Mutex *m, Thread *t)
{
  kassert(t != nullptr);
  kassert(m != nullptr);

  if (m->nthread_holding) {
    kassert(t == t->this_thread());
    m->nthread_holding--;
    if (!m->nthread_holding) {
      kassert(m->thread_holding);
      m->thread_holding = 0;
      if (pthread_mutex_unlock(&m->mutex) != 0) {
        Fatal("Pthread unlock error");
      }
    }
  }
}

class MutexLock
{
private:
  std::shared_ptr<Mutex> m;
  bool locked_p;

public:
  MutexLock(
#ifdef DEBUG
    const SourceLocation &location, const char *ahandler,
#endif
    Mutex *am, Thread *t)
    : m(am), locked_p(true)
  {
    Mutex_lock(
#ifdef DEBUG
      location, ahandler,
#endif
      m.get(), t);
  }

  MutexLock(
#ifdef DEBUG
    const SourceLocation &location, const char *ahandler,
#endif
    std::shared_ptr<Mutex> &am, Thread *t)
    : m(am), locked_p(true)
  {
    Mutex_lock(
#ifdef DEBUG
      location, ahandler,
#endif
      m.get(), t);
  }

  void
  release()
  {
    if (locked_p)
      Mutex_unlock(m.get(), m->thread_holding);
    locked_p = false;
  }

  ~MutexLock() { this->release(); }
};

class MutexTryLock
{
private:
  std::shared_ptr<Mutex> m;
  bool lock_acquired;

public:
  MutexTryLock(
#ifdef DEBUG
    const SourceLocation &location, const char *ahandler,
#endif
    Mutex *am, Thread *t)
    : m(am)
  {
    lock_acquired = Mutex_trylock(
#ifdef DEBUG
      location, ahandler,
#endif
      m.get(), t);
  }

  MutexTryLock(
#ifdef DEBUG
    const SourceLocation &location, const char *ahandler,
#endif
    std::shared_ptr<Mutex> &am, Thread *t)
    : m(am)
  {
    lock_acquired = Mutex_trylock(
#ifdef DEBUG
      location, ahandler,
#endif
      m.get(), t);
  }

  MutexTryLock(
#ifdef DEBUG
    const SourceLocation &location, const char *ahandler,
#endif
    Mutex *am, Thread *t, int sp)
    : m(am)
  {
    lock_acquired = Mutex_trylock_spin(
#ifdef DEBUG
      location, ahandler,
#endif
      m.get(), t, sp);
  }

  MutexTryLock(
#ifdef DEBUG
    const SourceLocation &location, const char *ahandler,
#endif
    std::shared_ptr<Mutex> &am, Thread *t, int sp)
    : m(am)
  {
    lock_acquired = Mutex_trylock_spin(
#ifdef DEBUG
      location, ahandler,
#endif
      m.get(), t, sp);
  }

  ~MutexTryLock()
  {
    if (lock_acquired)
      Mutex_unlock(m.get(), m->thread_holding);
  }

  void
  acquire(Thread *t)
  {
    MUTEX_TAKE_LOCK(m.get(), t);
    lock_acquired = true;
  }

  void
  release()
  {
    kassert(lock_acquired); // generate a warning because it shouldn't be done.
    if (lock_acquired) {
      Mutex_unlock(m.get(), m->thread_holding);
    }
    lock_acquired = false;
  }

  bool
  is_locked() const
  {
    return lock_acquired;
  }

  const Mutex *
  get_mutex() const
  {
    return m.get();
  }
};

#endif // PROJECT_I_MUTEX_H
