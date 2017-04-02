//
// Created by 宋辰伟 on 2017/4/2.
//

#include "P_EventSystem.h"

#include "king/king_string.h"
#include "king/king_memory.h"

struct thread_ctx {
  ThreadFunction f;
  void *data;
  Thread *me;
  char name[MAX_THREAD_NAME_LENGTH];
};

khrtime Thread::cur_time = 0;

static kthread_key init_thread_key();
kcoreapi kthread_key Thread::thread_data_key = init_thread_key();

Thread::Thread()
{
  //   mutex = new_ProxyMutex();
  //   MUTEX_TAKE_LOCK(mutex, (EThread *)this);
  //   mutex->nthread_holding = THREAD_MUTEX_THREAD_HOLDING;
}

static void
key_destructor(void *value)
{
  (void)value;
}

kthread_key
init_thread_key()
{
  kthread_key_create(&Thread::thread_data_key, key_destructor);
  return Thread::thread_data_key;
}

static void *
spawn_thread_internal(void *a)
{
  thread_ctx *p = (thread_ctx *)a;
  p->me->set_specific();
  kset_thread_name(p->name);
  if (p->f)
    p->f(p->data);
  else
    p->me->execute();
  kfree(a);
  printf("thread start %s", p->name);
  return nullptr;
}

kthread
Thread::start(const char *name, size_t stacksize, ThreadFunction f, void *a, void *stack)
{
  thread_ctx *ctx = (thread_ctx *)malloc(sizeof(thread_ctx));

  ctx->f    = f;
  ctx->data = a;
  ctx->me   = this;
  memset(ctx->name, 0, MAX_THREAD_NAME_LENGTH);
  kstrlcpy(ctx->name, name, MAX_THREAD_NAME_LENGTH);

  tid = kthread_create(spawn_thread_internal, (void *)ctx, 0, stacksize, stack);

  return tid;
}
