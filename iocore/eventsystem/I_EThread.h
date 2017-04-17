
#ifndef TEST_LOCK_I_ETHREAD_H
#define TEST_LOCK_I_ETHREAD_H

#include "EventSystem.h"
#include "I_Thread.h"
#include "I_Event.h"
#include "ink_rand.h"

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
  Event *schedule_at(Continuation *c, ktime atimeout_at, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_in(Continuation *c, ktime atimeout_in, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_every(Continuation *c, ktime aperiod, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_imm_local(Continuation *c, int callback_event = EVENT_IMMEDIATE, void *cookie = nullptr);
  Event *schedule_at_local(Continuation *c, ktime atimeout_at, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_in_local(Continuation *c, ktime atimeout_in, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
  Event *schedule_every_local(Continuation *c, ktime aperiod, int callback_event = EVENT_INTERVAL, void *cookie = nullptr);
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
};

#endif
