
#ifndef TEST_LOCK_I_ETHREAD_H
#define TEST_LOCK_I_ETHREAD_H

#include "I_Thread.h"
#include "ink_rand.h"
#include "I_ProtectedQueue.h"

#define PER_THREAD_DATA (1024 * 1024)
#define MUTEX_RETRY_DELAY HRTIME_MSECONDS(20)

class Event;
class Continuation;

enum ThreadType {
  REGULAR = 0,
  MONITOR,
  DEDICATED,
};

class EThread : public Thread
{
public:
  Event *schedule_imm(Continuation *c, int callback_event = EVENT_IMMEDIATE, void *cookie = nullptr);
  Event *schedule_imm_signal(Continuation *c, int callback_event = EVENT_IMMEDIATE, void *cookie = nullptr);
  Event *schedule_at(Continuation *c, khrtime atimeout_at, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_in(Continuation *c, khrtime atimeout_in, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_every(Continuation *c, khrtime aperiod, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_imm_local(Continuation *c, int callback_event = EVENT_IMMEDIATE, void *cookie = nullptr);
  Event *schedule_at_local(Continuation *c, khrtime atimeout_at, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_in_local(Continuation *c, khrtime atimeout_in, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_every_local(Continuation *c, khrtime aperiod, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_local(Event *e);

  InkRand generator;

private:
  // prevent unauthorized copies (Not implemented)
  EThread(const EThread &);
  EThread &operator=(const EThread &);

public:
  EThread();
  EThread(ThreadType att, int anid);
  EThread(ThreadType att, Event *e);

  virtual ~EThread() { mutex == nullptr; }
  Event *schedule_spawn(Continuation *cont);
  Event *schedule(Event *e, bool fast_signal = false);

  char thread_private[PER_THREAD_DATA];

  ProtectedQueue EventQueueExternal;
  //  PriorityEventQueue EventQueue;

  EThread **ethreads_to_be_signalled;
  int n_ethreads_to_be_signalled;

  int id;
  unsigned int event_types;
  bool is_event_type(EventType et);
  void set_event_type(EventType et);

  void execute();
  void process_event(Event *e, int calling_code);
  void free_event(Event *e);
  void (*signal_hook)(EThread *);

  ThreadType tt;
  Event *oneevent; // For dedicated event thread
};

#endif
