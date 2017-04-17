#ifndef _P_Thread_h_
#define _P_Thread_h_

#include "I_Thread.h"
inline Thread::~Thread()
{
	mutex = nullptr;
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
#endif //_P_Thread_h_
