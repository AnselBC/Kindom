//
// Created by 宋辰伟 on 2017/4/17.
//

#include "P_EventSystem.h"
#include <sched.h>
#include "hugepages.h"
#include "kmemory.h"

EventType
EventProcessor::spawn_event_threads(int n_threads, const char *et_name, size_t stacksize)
{
    char thr_name[MAX_THREAD_NAME_LENGTH];
    EventType new_thread_group_id;
    int i;

    krelease_assert(n_threads > 0);
    krelease_assert((n_ethreads + n_threads) <= MAX_EVENT_THREADS);
    krelease_assert(n_thread_groups < MAX_EVENT_TYPES);

    new_thread_group_id = (EventType)n_thread_groups;

    for (i = 0; i < n_threads; i++) {
        EThread *t                          = new EThread(REGULAR, n_ethreads + i);
        all_ethreads[n_ethreads + i]        = t;
        eventthread[new_thread_group_id][i] = t;
        t->set_event_type(new_thread_group_id);
    }


    n_threads_for_type[new_thread_group_id] = n_threads;
    for (i = 0; i < n_threads; i++) {
        snprintf(thr_name, MAX_THREAD_NAME_LENGTH, "[%s %d]", et_name, i);
        eventthread[new_thread_group_id][i]->start(thr_name, stacksize, nullptr, nullptr, nullptr);
    }

    n_thread_groups++;
    n_ethreads += n_threads;
    Debug("iocore_thread", "Created thread group '%s' id %d with %d threads", et_name, new_thread_group_id, n_threads);

    return new_thread_group_id;
}

static void *
alloc_stack(size_t stacksize)
{
    void *stack = nullptr;

    if (khugepage_enabled()) {
        stack = kalloc_hugepage(stacksize);
    }

    if (stack == nullptr) {
        stack = kmemalign(kpagesize(), stacksize);
    }

    return stack;
}

class EventProcessor eventProcessor;

int
EventProcessor::start(int n_event_threads, size_t stacksize) {
    char thr_name[MAX_THREAD_NAME_LENGTH];
    int i;
    void *stack = nullptr;

    // do some sanity checking.
    static int started = 0;
    krelease_assert(!started);
    krelease_assert(n_event_threads > 0 && n_event_threads <= MAX_EVENT_THREADS);
    started = 1;

    n_ethreads = n_event_threads;
    n_thread_groups = 1;

    stacksize = MAX(stacksize, INK_THREAD_STACK_MIN);

    if (ats_hugepage_enabled()) {
        stacksize = INK_ALIGN(stacksize, ats_hugepage_size());
    } else {
        stacksize = INK_ALIGN(stacksize, ats_pagesize());
    }

    Debug("iocore_thread", "Thread stack size set to %zu", stacksize);

    for (i = 0; i < n_event_threads; i++) {
        EThread *t = new EThread(REGULAR, i);
        all_ethreads[i] = t;

        eventthread[ET_CALL][i] = t;
        t->set_event_type((EventType) ET_CALL);
    }
    n_threads_for_type[ET_CALL] = n_event_threads;

    for (i = 0; i < n_ethreads; i++) {
        kthread tid;
        snprintf(thr_name, MAX_THREAD_NAME_LENGTH, "[ET_NET %d]", i);
        stack = alloc_stack(stacksize);

        tid   = all_ethreads[i]->start(thr_name, stacksize, nullptr, nullptr, stack);
    }

    Debug("iocore_thread", "Created event thread group id %d with %d threads", ET_CALL, n_event_threads);
    return 0;
}

void
EventProcessor::shutdown()
{
}

Event *
EventProcessor::spawn_thread(Continuation *cont, const char *thr_name, size_t stacksize)
{
    krelease_assert(n_dthreads < MAX_EVENT_THREADS);
    Event *e = new Event();

    e->init(cont, 0, 0);
    all_dthreads[n_dthreads] = new EThread(DEDICATED, e);
    e->ethread               = all_dthreads[n_dthreads];
    e->mutex = e->continuation->mutex = all_dthreads[n_dthreads]->mutex;
    n_dthreads++;
    e->ethread->start(thr_name, stacksize, nullptr, nullptr, nullptr);

    return e;
}