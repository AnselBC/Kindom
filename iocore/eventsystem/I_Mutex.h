//
// Created by 宋辰伟 on 2017/4/16.
//

#ifndef PROJECT_I_MUTEX_H
#define PROJECT_I_MUTEX_H

#include "EventSystem.h"

class Thread;
typedef ThreadPtr *Thread;

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

x_pthread_mutexattr_t _g_mattr;

class Mutex
{
public:
  kmutex mutex;
  volatile ThreadPtr thread_holding;
  int nthread_holding;
  int taken;

#ifdef DEBUG
  ktime holding_time;
  const char *file_name;
  const char *func_name;
#endif
  ~Mutex() { pthread_mutex_destroy(mutex); }
  Mutex()
  {
    thread_holding  = nullptr;
    nthread_holding = 0;
    taken           = 0;
#ifdef DEBUG
    file_name = nullptr;
    func_name = nullptr;
#endif
  }

  void
  init(const char *name = "UnnamedMutex")
  {
    if (pthread_mutex_init(m, &_g_mattr.attr) != 0) {
      abort();
    }
  }
};

inline bool
Mutex_trylock(
#ifdef DEBUG
  const char *filename, const char funcname,
#endif
  Mutex *m, Thread *t)
{
  kassert(t != nullptr);
  kassert(m != nullptr);
  kassert(t == t->this_thread());
  if (m->thread_holding != t) {
    if (pthread_mutex_trylock(m->mutex)) {
      return false;
    }
    m->thread_holding = t;
#ifdef DEBUG
    m->file_name = filename;
    m->func_name = funcname;
#endif
    m->taken++;
  }
  m->nthread_holding++;
  return true;
}

inline bool
Mutex_trylock_spin(
#ifdef DEBUG
  const char *filename, const char funcname,
#endif
  Mutex *m, Thread *t, int spincnt = 1)
{
  kassert(t != nullptr);
  kassert(m != nullptr);
  kassert(t == t->this_thread());
  if (m->thread_holding != t) {
    int locked;
    do {
      if (locked = (pthread_mutex_trylock(m->mutex) == 0))
        break;
    } while (--spincnt);
    if (!locked) {
      return false;
    }
    m->thread_holding = t;
#ifdef DEBUG
    m->file_name = filename;
    m->func_name = funcname;
#endif
    m->taken++;
  }
  m->nthread_holding++;
  return true;
}

inline int
Mutex_lock(
#ifdef DEBUG
  const char *filename, const char funcname,
#endif
  Mutex *m, Thread *t)
{
  kassert(t != nullptr);
  kassert(m != nullptr);
  kassert(t == t->this_thread());
  if (m->thread_holding != t) {
    if (pthread_mutex_lock(m->mutex) != 0) {
      Fatal("Pthread lock acquire error");
    }
    m->thread_holding = t;
#ifdef DEBUG
    m->file_name = filename;
    m->func_name = funcname;
#endif
    m->taken++;
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
      if (pthread_mutex_unlock(m->mutex) != 0) {
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
    const char *filename, const char funcname,
#endif
    Mutex *am, Thread *t)
    : m(am), locked_p(true)
  {
    Mutex_lock(
#ifdef DEBUG
      filename, funcname,
#endif
      m.get(), t);
  }

  MutexLock(
#ifdef DEBUG
    const char *filename, const char funcname,
#endif
    std::shared_ptr<Mutex> &am, Thread *t)
    : m(am), locked_p(true)
  {
    Mutex_lock(
#ifdef DEBUG
      filename, funcname,
#endif
      m.get(), t);
  }

  void
  release()
  {
    if (locked_p)
      Mutex_unlock(m, m->thread_holding);
    locked_p = false;
  }

  ~MutexLock() { this->release(); }
};

class MutexTryLock
{
private:
  Ptr<ProxyMutex> m;
  bool lock_acquired;

public:
  MutexTryLock(
#ifdef DEBUG
    const char *filename, const char funcname,
#endif
    Mutex *am, Thread *t)
    : m(am)
  {
    lock_acquired = Mutex_trylock(
#ifdef DEBUG
      filename, funcname,
#endif
      m.get(), t);
  }

  MutexTryLock(
#ifdef DEBUG
    const char *filename, const char funcname,
#endif
    std::shared_ptr<Mutex> &am, Thread *t)
    : m(am)
  {
    lock_acquired = Mutex_trylock(
#ifdef DEBUG
      filename, funcname,
#endif
      m.get(), t);
  }

  MutexTryLock(
#ifdef DEBUG
    const char *filename, const char funcname,
#endif
    Mutex *am, Thread *t, int sp)
    : m(am)
  {
    lock_acquired = Mutex_trylock_spin(
#ifdef DEBUG
      filename, funcname,
#endif
      m.get(), t, sp);
  }

  MutexTryLock(
#ifdef DEBUG
    const char *filename, const char funcname,
#endif
    std::shared_ptr<Mutex> &am, Thread *t, int sp)
    : m(am)
  {
    lock_acquired = Mutex_trylock_spin(
#ifdef DEBUG
      filename, funcname,
#endif
      m.get(), t, sp);
  }

  ~MutexTryLock()
  {
    if (lock_acquired)
      Mutex_unlock(m.get(), m->thread_holding);
  }

  void
  acquire(EThread *t)
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
