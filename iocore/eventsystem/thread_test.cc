//
// Created by 宋辰伟 on 2017/4/2.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "I_EventSystem.h"

static void *
thread_start(void *arg)
{
  printf("hello start! %ld\n", *(int64_t *)(arg));
}

int
main()
{
  Thread *thread = new Thread();
  thread->get_hrtime_updated();
  static int64_t tm = thread->get_hrtime();
  thread->start("hello", 0, thread_start, &tm, nullptr);
  sleep(1);
}
