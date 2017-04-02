//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef KINDOM_KING_THREAD_H
#define KINDOM_KING_THREAD_H

#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#include "king/king_error.h"
#include "king/king_hrtime.h"

#define KMUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define KTHREAD_STACK_MIN PTHREAD_STACK_MIN

typedef pthread_t kthread;
typedef pthread_cond_t kcond;
typedef pthread_key_t kthread_key;

static inline void
kthread_key_create(kthread_key *key, void (*destructor)(void *value))
{
    kassert(!pthread_key_create(key, destructor));
}


static inline void
kthread_setspecific(kthread_key key, void *value)
{
    kassert(!pthread_setspecific(key, value));
}

inline kthread
kthread_create(void *(*f)(void *), void *a, int detached, size_t stacksize, void *stack)
{
  kthread t;
  int ret;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  if (stacksize) {
    if (stack) {
      pthread_attr_setstack(&attr, stack, stacksize);
    } else {
      pthread_attr_setstacksize(&attr, stacksize);
    }
  }

  if (detached) {
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  }

  ret = pthread_create(&t, &attr, f, a);
  if (ret != 0) {
    //        king_abort("pthread_create() failed: %s (%d)", strerror(ret), ret);
  }
  pthread_attr_destroy(&attr);

  return t;
}

#endif // KINDOM_KING_THREAD_H
