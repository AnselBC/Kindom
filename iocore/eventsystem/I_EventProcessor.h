//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_I_EVENTPROCESSOR_H
#define TEST_LOCK_I_EVENTPROCESSOR_H

#include "I_Continuation.h"
#include "I_Processor.h"
#include "I_Event.h"

#ifdef KMAX_THREADS_IN_EACH_THREAD_TYPE
const int MAX_THREADS_IN_EACH_TYPE = KMAX_THREADS_IN_EACH_THREAD_TYPE;
#else
const int MAX_THREADS_IN_EACH_TYPE = 3072;
#endif

#ifdef KMAX_NUMBER_EVENT_THREADS
const int MAX_EVENT_THREADS = KMAX_NUMBER_EVENT_THREADS;
#else
const int MAX_EVENT_THREADS = 4096;
#endif

class EThread;

class EventProcessor : public Processor
{
public:
  Event *spawn_thread(Continuation *cont, const char *thr_name, size_t stacksize = 0);
  EventType spawn_event_threads(int n_threads, const char *et_name, size_t stacksize);
  Event *schedule_imm(Continuation *c, EventType event_type = ET_CALL, int callback_event = EVENT_IMMEDIATE,
                      void *cookie = nullptr);
  Event *schedule_imm_signal(Continuation *c, EventType event_type = ET_CALL, int callback_event = EVENT_IMMEDIATE,
                             void *cookie = nullptr);
  Event *schedule_at(Continuation *c, khrtime atimeout_at, EventType event_type = ET_CALL, int callback_event = EVENT_INTERVAL,
                     void *cookie = nullptr);
  Event *schedule_in(Continuation *c, khrtime atimeout_in, EventType event_type = ET_CALL, int callback_event = EVENT_INTERVAL,
                     void *cookie = nullptr);
  Event *schedule_every(Continuation *c, khrtime aperiod, EventType event_type = ET_CALL, int callback_event = EVENT_INTERVAL,
                        void *cookie = nullptr);
  Event *reschedule_imm(Event *e, int callback_event = EVENT_IMMEDIATE);
  Event *reschedule_at(Event *e, khrtime atimeout_at, int callback_event = EVENT_INTERVAL);
  Event *reschedule_in(Event *e, khrtime atimeout_in, int callback_event = EVENT_INTERVAL);
  Event *reschedule_every(Event *e, khrtime aperiod, int callback_event = EVENT_INTERVAL);

  EventProcessor();

  int start(int n_net_threads, size_t stacksize = DEFAULT_STACKSIZE);

  virtual void shutdown();

  off_t allocate(int size);

  EThread *all_ethreads[MAX_EVENT_THREADS];

  EThread *eventthread[MAX_EVENT_TYPES][MAX_THREADS_IN_EACH_TYPE];

  unsigned int next_thread_for_type[MAX_EVENT_TYPES];

  int n_threads_for_type[MAX_EVENT_TYPES];

  int n_ethreads;

  int n_thread_groups;

private:
  // prevent unauthorized copies (Not implemented)
  EventProcessor(const EventProcessor &);
  EventProcessor &operator=(const EventProcessor &);

public:
  /*------------------------------------------------------*\
  | Unix & non NT Interface                                |
  \*------------------------------------------------------*/

  Event *schedule(Event *e, EventType etype, bool fast_signal = false);
  EThread *assign_thread(EventType etype);

  EThread *all_dthreads[MAX_EVENT_THREADS];
  int n_dthreads; // No. of dedicated threads
  volatile int thread_data_used;
};

extern class EventProcessor eventProcessor;

#endif // TEST_LOCK_I_EVENTPROCESSOR_H
