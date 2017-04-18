//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_P_EVENTPROCESSOR_H
#define TEST_LOCK_P_EVENTPROCESSOR_H

#include "kalign.h"
#include "I_EventProcessor.h"
#include <stddef.h>

inline EventProcessor::EventProcessor() : n_ethreads(0), n_thread_groups(0), n_dthreads(0), thread_data_used(0)
{
  memset(all_ethreads, 0, sizeof(all_ethreads));
  memset(all_dthreads, 0, sizeof(all_dthreads));
  memset(n_threads_for_type, 0, sizeof(n_threads_for_type));
  memset(next_thread_for_type, 0, sizeof(next_thread_for_type));
}

inline off_t
EventProcessor::allocate(int size)
{
  static off_t start = KALIGN(offsetof(EThread, thread_private), 16);
  static off_t loss  = start - offsetof(EThread, thread_private);
  size               = KALIGN(size, 16); // 16 byte alignment

  int old;
  do {
    old = thread_data_used;
    if (old + loss + size > PER_THREAD_DATA)
      return -1;
  } while (!katomic_cas(&thread_data_used, old, old + size));

  return (off_t)(old + start);
}

inline EThread *
EventProcessor::assign_thread(EventType etype)
{
  int next;

  kassert(etype < MAX_EVENT_TYPES);
  if (n_threads_for_type[etype] > 1)
    next = next_thread_for_type[etype]++ % n_threads_for_type[etype];
  else
    next = 0;
  return (eventthread[etype][next]);
}

inline Event *
EventProcessor::schedule(Event *e, EventType etype, bool fast_signal)
{
  kassert(etype < MAX_EVENT_TYPES);
  e->ethread = assign_thread(etype);
  if (e->continuation->mutex)
    e->mutex = e->continuation->mutex;
  else
    e->mutex = e->continuation->mutex = e->ethread->mutex;
  e->ethread->EventQueueExternal.enqueue(e, fast_signal);
  return e;
}

inline Event *
EventProcessor::schedule_imm_signal(Continuation *cont, EventType et, int callback_event, void *cookie)
{
  Event *e = new Event();

  kassert(et < MAX_EVENT_TYPES);
#ifdef ENABLE_TIME_TRACE
  e->start_time = Thread::get_hrtime();
#endif
  e->callback_event = callback_event;
  e->cookie         = cookie;
  return schedule(e->init(cont, 0, 0), et, true);
}

inline Event *
EventProcessor::schedule_imm(Continuation *cont, EventType et, int callback_event, void *cookie)
{
  Event *e = new Event();

  kassert(et < MAX_EVENT_TYPES);
#ifdef ENABLE_TIME_TRACE
  e->start_time = Thread::get_hrtime();
#endif
  e->callback_event = callback_event;
  e->cookie         = cookie;
  return schedule(e->init(cont, 0, 0), et);
}

inline Event *
EventProcessor::schedule_at(Continuation *cont, khrtime t, EventType et, int callback_event, void *cookie)
{
  Event *e = new Event();

  kassert(t > 0);
  kassert(et < MAX_EVENT_TYPES);
  e->callback_event = callback_event;
  e->cookie         = cookie;
  return schedule(e->init(cont, t, 0), et);
}

inline Event *
EventProcessor::schedule_in(Continuation *cont, khrtime t, EventType et, int callback_event, void *cookie)
{
  Event *e = new Event();

  kassert(et < MAX_EVENT_TYPES);
  e->callback_event = callback_event;
  e->cookie         = cookie;
  return schedule(e->init(cont, Thread::get_hrtime() + t, 0), et);
}

inline Event *
EventProcessor::schedule_every(Continuation *cont, khrtime t, EventType et, int callback_event, void *cookie)
{
  Event *e = new Event();

  kassert(t != 0);
  kassert(et < MAX_EVENT_TYPES);
  e->callback_event = callback_event;
  e->cookie         = cookie;
  if (t < 0)
    return schedule(e->init(cont, t, t), et);
  else
    return schedule(e->init(cont, Thread::get_hrtime() + t, t), et);
}

#endif // TEST_LOCK_P_EVENTPROCESSOR_H
