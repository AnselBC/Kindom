//
// Created by 宋辰伟 on 2017/4/17.
//

#include "P_EventSystem.h"

void
ProtectedQueue::enqueue(Event *e, bool fast_signal)
{
  kassert(!e->in_the_priority_queue && !e->in_the_prot_queue);
  EThread *e_thr       = e->ethread;
  e->in_the_prot_queue = 1;
  bool was_empty       = (katomiclist_push(&al, e) == nullptr);
  if (was_empty) {
    EThread *inserting_thread = this_ethread();
    if (inserting_thread != e_ethread) {
      if (!inserting_thread || !inserting_thread->ethreads_to_be_signalled) {
        signal();
        if (fast_signal) {
          if (e_ethread->signal_hook)
            e_ethread->signal_hook(e_ethread);
        }
      } else {
#ifdef EAGER_SIGNALLING
        // Try to signal now and avoid deferred posting.
        if (e_ethread->EventQueueExternal.try_signal())
          return;
#endif
        if (fast_signal) {
          if (e_ethread->signal_hook)
            e_ethread->signal_hook(e_ethread);
        }

        int &t          = inserting_thread->n_ethreads_to_be_signalled;
        EThread **sig_e = inserting_thread->ethreads_to_be_signalled;
        if ((t + 1) >= eventProcessor.n_ethreads) {
          // we have run out of room
          if ((t + 1) == eventProcessor.n_ethreads) {
            // convert to direct map, put each ethread (sig_e[i]) into
            // the direct map loation: sig_e[sig_e[i]->id]
            for (int i = 0; i < t; i++) {
              EThread *cur = sig_e[i]; // put this ethread
              while (cur) {
                EThread *next = sig_e[cur->id]; // into this location
                if (next == cur)
                  break;
                sig_e[cur->id] = cur;
                cur            = next;
              }
              // if not overwritten
              if (sig_e[i] && sig_e[i]->id != i)
                sig_e[i] = nullptr;
            }
            t++;
          }
          // we have a direct map, insert this EThread
          sig_e[e_ethread->id] = e_ethread;
        } else
          // insert into vector
          sig_e[t++] = e_ethread;
      }
    }
  }
}

void
flush_signals(EThread *thr)
{
  kassert(this_ethread() == thr);
  int n = thr->n_ethreads_to_be_signalled;
  if (n > eventProcessor.n_ethreads)
    n = eventProcessor.n_ethreads; // MAX
  int i;

// Since the lock is only there to prevent a race in kcond_timedwait
// the lock is taken only for a short time, thus it is unlikely that
// this code has any effect.
#ifdef EAGER_SIGNALLING
  for (i = 0; i < n; i++) {
    // Try to signal as many threads as possible without blocking.
    if (thr->ethreads_to_be_signalled[i]) {
      if (thr->ethreads_to_be_signalled[i]->EventQueueExternal.try_signal())
        thr->ethreads_to_be_signalled[i] = 0;
    }
  }
#endif
  for (i = 0; i < n; i++) {
    if (thr->ethreads_to_be_signalled[i]) {
      thr->ethreads_to_be_signalled[i]->EventQueueExternal.signal();
      if (thr->ethreads_to_be_signalled[i]->signal_hook)
        thr->ethreads_to_be_signalled[i]->signal_hook(thr->ethreads_to_be_signalled[i]);
      thr->ethreads_to_be_signalled[i] = nullptr;
    }
  }
  thr->n_ethreads_to_be_signalled = 0;
}

void
ProtectedQueue::dequeue_timed(khrtime cur_time, khrtime timeout, bool sleep)
{
  (void)cur_time;
  Event *e;
  if (sleep) {
    kmutex_acquire(&lock);
    if (INK_ATOMICLIST_EMPTY(al)) {
      timespec ts = khrtime_to_timespec(timeout);
      kcond_timedwait(&might_have_data, &lock, &ts);
    }
    kmutex_release(&lock);
  }

  e = (Event *)katomiclist_popall(&al);
  // invert the list, to preserve order
  SLL<Event, Event::Lklink> l, t;
  t.head = e;
  while ((e = t.pop()))
    l.push(e);
  // insert into localQueue
  while ((e = l.pop())) {
    if (!e->cancelled)
      localQueue.enqueue(e);
    else {
      e->mutex = nullptr;
      eventAllocator.free(e);
    }
  }
}