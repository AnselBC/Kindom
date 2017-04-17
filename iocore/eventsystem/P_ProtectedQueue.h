//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_P_PROTECTEDQUEUE_H
#define TEST_LOCK_P_PROTECTEDQUEUE_H

#include "I_EventSystem.h"

inline
ProtectedQueue::ProtectedQueue()
{
    Event e;
    kmutex_init(&lock, "ProtectedQueue");
    katomiclist_init(&al, "ProtectedQueue", (char *)&e.link.next - (char *)&e);
    kcond_init(&might_have_data);
}

inline void
ProtectedQueue::signal()
{
    // Need to get the lock before you can signal the thread
    kmutex_acquire(&lock);
    kcond_signal(&might_have_data);
    kmutex_release(&lock);
}

inline int
ProtectedQueue::try_signal()
{
    // Need to get the lock before you can signal the thread
    if (kmutex_try_acquire(&lock)) {
        kcond_signal(&might_have_data);
        kmutex_release(&lock);
        return 1;
    } else {
        return 0;
    }
}

// Called from the same thread (don't need to signal)
inline void
ProtectedQueue::enqueue_local(Event *e)
{
    kassert(!e->in_the_prot_queue && !e->in_the_priority_queue);
    e->in_the_prot_queue = 1;
    localQueue.enqueue(e);
}

inline void
ProtectedQueue::remove(Event *e)
{
    kassert(e->in_the_prot_queue);
    if (!katomiclist_remove(&al, e))
        localQueue.remove(e);
    e->in_the_prot_queue = 0;
}

inline Event *
ProtectedQueue::dequeue_local()
{
    Event *e = localQueue.dequeue();
    if (e) {
        kassert(e->in_the_prot_queue);
        e->in_the_prot_queue = 0;
    }
    return e;
}

#endif //TEST_LOCK_P_PROTECTEDQUEUE_H
