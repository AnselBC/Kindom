#ifndef _KTHREAD_H
#define _KTHREAD_H

#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#include "kmutex.h"

typedef struct timespec ktimestruc;

typedef pthread_t kthread;
typedef pthread_cond_t kcond;
typedef pthread_key_t kthread_key;

struct ksemaphore {
#if TS_EMULATE_ANON_SEMAPHORES
  sem_t *sema;
  int64_t semid;
#else
  sem_t sema;
#endif

  sem_t *
  get()
  {
#if TS_EMULATE_ANON_SEMAPHORES
    return sema;
#else
    return &sema;
#endif
  }
};

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

static inline void *
kthread_getspecific(kthread_key key)
{
  return pthread_getspecific(key);
}

static inline void
kthread_key_delete(kthread_key key)
{
  kassert(!pthread_key_delete(key));
}

static inline kthread
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
    Fatal("pthread_create() failed: %s (%d)", strerror(ret), ret);
  }
  pthread_attr_destroy(&attr);

  return t;
}

static inline void
kthread_cancel(kthread who)
{
#if defined(freebsd)
  (void)who;
  kassert(!"not supported");
#else
  int ret = pthread_cancel(who);
  kassert(ret == 0);
#endif
}

static inline void *
kthread_join(kthread t)
{
  void *r;
  kassert(!pthread_join(t, &r));
  return r;
}

static inline kthread
kthread_self()
{
  return (pthread_self());
}

static inline kthread
kthread_null()
{
  return (kthread)0;
}

static inline int
kthread_get_priority(kthread t, int *priority)
{
#if defined(freebsd)
  (void)t;
  (void)priority;
  kassert(!"not supported");
  return -1;
#else
  int policy;
  struct sched_param param;
  int res   = pthread_getschedparam(t, &policy, &param);
  *priority = param.sched_priority;
  return res;
#endif
}

static inline int
kthread_sigsetmask(int how, const sigset_t *set, sigset_t *oset)
{
  return (pthread_sigmask(how, set, oset));
}

void ksem_init(ksemaphore *sp, unsigned int count);
void ksem_destroy(ksemaphore *sp);
void ksem_wait(ksemaphore *sp);
bool ksem_trywait(ksemaphore *sp);
void ksem_post(ksemaphore *sp);

static inline void
kcond_init(kcond *cp)
{
  kassert(pthread_cond_init(cp, nullptr) == 0);
}

static inline void
kcond_destroy(kcond *cp)
{
  kassert(pthread_cond_destroy(cp) == 0);
}

static inline void
kcond_wait(kcond *cp, kmutex *mp)
{
  kassert(pthread_cond_wait(cp, mp) == 0);
}
static inline int
kcond_timedwait(kcond *cp, kmutex *mp, ktimestruc *t)
{
  int err;
  while (EINTR == (err = pthread_cond_timedwait(cp, mp, t)))
    ;
#if defined(freebsd) || defined(openbsd)
  kassert((err == 0) || (err == ETIMEDOUT));
#else
  kassert((err == 0) || (err == ETIME) || (err == ETIMEDOUT));
#endif
  return err;
}

static inline void
kcond_signal(kcond *cp)
{
  kassert(pthread_cond_signal(cp) == 0);
}

static inline void
kcond_broadcast(kcond *cp)
{
  kassert(pthread_cond_broadcast(cp) == 0);
}

static inline void
kthr_yield()
{
  kassert(!sched_yield());
}

static inline void
kthread_exit(void *status)
{
  pthread_exit(status);
}

// This define is from Linux's <sys/prctl.h> and is most likely very
// Linux specific... Feel free to add support for other platforms
// that has a feature to give a thread specific name / tag.
static inline void
kset_thread_name(const char *name)
{
  pthread_setname_np(pthread_self(), name);
}

#endif
