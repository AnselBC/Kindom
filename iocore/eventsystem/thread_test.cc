#include "P_EventSystem.h"

std::shared_ptr<Mutex> mutex;

static void *
thread_main(void *thread)
{
  printf("thread start!!!\n");
  Thread *t = (Thread *)thread;
  t->get_hrtime_updated();
  MUTEX_TRY_LOCK(lock, mutex, t);
  if (lock.is_locked()) {
    printf("lock %ld\n", t->get_hrtime());
  } else {
    printf("unlock\n");
  }
  sleep(1);
}
int
main()
{
  std::shared_ptr<Mutex> m(new Mutex);
  mutex = m;

  Thread *thread1 = new Thread();
  Thread *thread2 = new Thread();
  thread1->start("[scw1]", 0, thread_main, thread1, nullptr);
  thread2->start("[scw2]", 0, thread_main, thread1, nullptr);
  sleep(3);
}
