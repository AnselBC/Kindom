//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef PROJECT_KING_MUTEX_H
#define PROJECT_KING_MUTEX_H


#include <stdio.h>

#include "king/king_defs.h"

#if defined(POSIX_THREAD)
#include <pthread.h>
#include <stdlib.h>

typedef pthread_mutex_t kmutex;

// just a wrapper so that the constructor gets executed
// before the first call to ink_mutex_init();
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

#endif /* #if defined(POSIX_THREAD) */
#endif //PROJECT_KING_MUTEX_H
