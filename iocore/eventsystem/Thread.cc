//
// Created by 宋辰伟 on 2017/4/2.
//

#include "P_EventSystem.h"

#include "king/king_string.h"

struct thread_ctx {
  ThreadFunction f;
  void *data;
  Thread *me;
  char name[MAX_THREAD_NAME_LENGTH];
};

Thread::Thread()
{
  //   mutex = new_ProxyMutex();
  //   MUTEX_TAKE_LOCK(mutex, (EThread *)this);
  //   mutex->nthread_holding = THREAD_MUTEX_THREAD_HOLDING;
}

static void *
spawn_thread_internal(void *a)
{
  thread_ctx *p = (thread_ctx *)a;

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
  strcpy(ctx->name, name);

  tid = kthread_create(spawn_thread_internal, (void *)ctx, 0, stacksize, stack);

  return tid;
}
