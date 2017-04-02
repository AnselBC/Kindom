//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef KINDOM_P_THREAD_H
#define KINDOM_P_THREAD_H

#include "I_Thread.h"

inline Thread::~Thread()
{
}

inline void
Thread::set_specific()
{
  kthread_setspecific(Thread::thread_data_key, this);
}

inline Thread *
this_thread()
{
  return (Thread *)kthread_getspecific(Thread::thread_data_key);
}

#endif // KINDOM_P_THREAD_H
