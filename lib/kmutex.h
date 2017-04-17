//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_KMUTEX_H
#define TEST_LOCK_KMUTEX_H

#include <stdio.h>

#include <pthread.h>
#include <stdlib.h>

typedef pthread_mutex_t kmutex;

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

extern class x_pthread_mutexattr_t _g_mattr;

static inline int
kmutex_init(kmutex *m, const char *name)
{
  (void)name;

#if defined(solaris)
  if (pthread_mutex_init(m, nullptr) != 0) {
    abort();
  }
#else
  if (pthread_mutex_init(m, &_g_mattr.attr) != 0) {
    abort();
  }
#endif
  return 0;
}

static inline int
kmutex_destroy(kmutex *m)
{
  return pthread_mutex_destroy(m);
}

static inline int
kmutex_acquire(kmutex *m)
{
  if (pthread_mutex_lock(m) != 0) {
    abort();
  }
  return 0;
}

static inline int
kmutex_release(kmutex *m)
{
  if (pthread_mutex_unlock(m) != 0) {
    abort();
  }
  return 0;
}

static inline int
kmutex_try_acquire(kmutex *m)
{
  return pthread_mutex_trylock(m) == 0;
}

#endif //TEST_LOCK_KMUTEX_H
