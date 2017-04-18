//
// Created by 宋辰伟 on 2017/4/16.
//

#include "P_EventSystem.h"
#include "kmemory.h"

static kthread_key init_thread_key();

khrtime Thread::cur_time            = 0;
kthread_key Thread::thread_data_key = init_thread_key();

kthread_key
init_thread_key()
{
  kthread_key_create(&Thread::thread_data_key, nullptr);
  return Thread::thread_data_key;
}

typedef struct {
  ThreadFunction f;
  void *a;
  Thread *me;
  char name[MAX_THREAD_NAME];
} thread_internal_data;

static void *
spawn_thread_internal(void *a)
{
  thread_internal_data *p = (thread_internal_data *)a;

  p->me->set_specific();
  kset_thread_name(p->name);
  if (p->f)
    p->f(p->a);
  else
    p->me->execute();
  kfree(a);
  return nullptr;
}

kthread
Thread::start(const char *name, size_t stacksize, ThreadFunction f, void *a, void *stack)
{
  thread_internal_data *p = (thread_internal_data *)kmalloc(sizeof(thread_internal_data));
  p->f                    = f;
  p->a                    = a;
  p->me                   = this;
  memset(p->name, 0, MAX_THREAD_NAME);
  // FixMe: name should not be large than MAX_THREAD_NAME
  strncpy(p->name, name, strlen(name));

  if (stacksize == 0) {
    stacksize = DEFAULT_STACKSIZE;
  }

  tid = kthread_create(spawn_thread_internal, p, 0, stacksize, stack);

  return tid;
}

Thread::Thread()
{
  std::shared_ptr<Mutex> m(new Mutex);
  mutex = m;
}
