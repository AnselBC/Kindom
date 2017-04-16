//
// Created by 宋辰伟 on 2017/4/16.
//

#include "EventSystem.h"
#include "Kindom.h"
static kthread_key init_thread_key();

ktime Thread::cur_time = 0;
kthread_key Thread::thread_data_key = init_thread_key();

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

static kthread
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


kthread
Thread::start(const char *name, size_t stacksize, ThreadFunction f, void *a, void *stack)
{
	thread_internal_data *p = (thread_internal_data *)kmalloc(sizeof(thread_internal_data));			
	p->f = f;
	p->a = a;
	p->me = this;
	memset(p->name, 0, MAX_THREAD_NAME);
	// FixMe: name should not be large than MAX_THREAD_NAME
  strncpy(p->name, name, strlen(name));	
	
	if (stacksize == 0) {
    stacksize = DEFAULT_STACKSIZE;
  }
 
	tid = kthread_create(spawn_thread_internal, p, 0, stacksize, stack);	
	
	return tid;
}

