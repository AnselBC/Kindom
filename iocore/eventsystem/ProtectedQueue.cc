//
// Created by 宋辰伟 on 2017/4/17.
//

void
ProtectedQueue::enqueue(Event *e, bool fast_signal)
{
  kassert(!e->in_the_priority_queue && !e->in_the_prot_queue);
  EThread *e_thr       = e->ethread;
  e->in_the_prot_queue = 1;
  bool was_empty       = (ink_atomiclist_push(&al, e) == nullptr);
}