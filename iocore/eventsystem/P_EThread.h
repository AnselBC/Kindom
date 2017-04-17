#ifndef _P_EThread_h_
#define _P_EThread_h_

#include "I_EThread.h"

inline Event *
EThread::schedule_spawn(Continuation *cont)
{
}

inline EThread *
this_ethread()
{
  return (EThread *)this_thread();
}

inline void
EThread::free_event(Event *e)
{
  kassert(!e->in_the_priority_queue && !e->in_the_prot_queue);
  e->mutex = nullptr;
	delete e;
}

#endif
