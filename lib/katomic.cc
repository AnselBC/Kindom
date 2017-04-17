//
// Created by 宋辰伟 on 2017/4/17.
//

#include "katomic.h"

#define ADDRESS_OF_NEXT(x, offset) ((void **)((char *)x + offset))

void
katomiclist_init(KAtomicList *l, const char *name, uint32_t offset_to_next)
{
  l->name   = name;
  l->offset = offset_to_next;
  SET_FREELIST_POINTER_VERSION(l->head, FROM_PTR(0), 0);
}

void *
katomiclist_pop(KAtomicList *l)
{
  head_p item;
  head_p next;
  int result = 0;
  do {
    KQUEUE_LD(item, l->head);                      // item = l->head
    if (TO_PTR(FREELIST_POINTER(item)) == nullptr) // return nullptr if empty
      return nullptr;
    SET_FREELIST_POINTER_VERSION(next, *ADDRESS_OF_NEXT(TO_PTR(FREELIST_POINTER(item)), l->offset), FREELIST_VERSION(item) + 1);
    result = katomic_cas(&l->head.data, item.data, next.data);
  } while (result == 0);
  {
    void *ret = TO_PTR(FREELIST_POINTER(item));
    *ADDRESS_OF_NEXT(ret, l->offset) = nullptr;
    return ret;
  }
}

void *
katomiclist_popall(KAtomicList *l)
{
  head_p item;
  head_p next;
  int result = 0;
  do {
    KQUEUE_LD(item, l->head);
    if (TO_PTR(FREELIST_POINTER(item)) == nullptr)
      return nullptr;
    SET_FREELIST_POINTER_VERSION(next, FROM_PTR(nullptr), FREELIST_VERSION(item) + 1);
    result = katomic_cas(&l->head.data, item.data, next.data);
  } while (result == 0);
  {
    void *ret = TO_PTR(FREELIST_POINTER(item));
    void *e   = ret;
    /* fixup forward pointers */
    while (e) {
      void *n = TO_PTR(*ADDRESS_OF_NEXT(e, l->offset));
      *ADDRESS_OF_NEXT(e, l->offset) = n;
      e = n;
    }
    return ret;
  }
}

void *
katomiclist_push(KAtomicList *l, void *item)
{
  volatile void **adr_of_next = (volatile void **)ADDRESS_OF_NEXT(item, l->offset);
  head_p head;
  head_p item_pair;
  int result       = 0;
  volatile void *h = nullptr;
  do {
    KQUEUE_LD(head, l->head); // header == l->header
    h            = FREELIST_POINTER(head);
    *adr_of_next = h;
    kassert(item != TO_PTR(h));
    SET_FREELIST_POINTER_VERSION(item_pair, FROM_PTR(item), FREELIST_VERSION(head));
    KMEMORY_BARRIER;
    result = katomic_cas(&l->head.data, head.data, item_pair.data);
  } while (result == 0);

  return TO_PTR(h);
}

void *
katomiclist_remove(KAtomicList *l, void *item)
{
  head_p head;
  void *prev       = nullptr;
  void **addr_next = ADDRESS_OF_NEXT(item, l->offset);
  void *item_next  = *addr_next;
  int result       = 0;

  /*
   * first, try to pop it if it is first
   */
  KQUEUE_LD(head, l->head);
  while (TO_PTR(FREELIST_POINTER(head)) == item) {
    head_p next;
    SET_FREELIST_POINTER_VERSION(next, item_next, FREELIST_VERSION(head) + 1);
    result = katomic_cas(&l->head.data, head.data, next.data);

    if (result) {
      *addr_next = nullptr;
      return item;
    }
    KQUEUE_LD(head, l->head);
  }

  /*
   * then, go down the list, trying to remove it
   */
  prev = TO_PTR(FREELIST_POINTER(head));
  while (prev) {
    void **prev_adr_of_next = ADDRESS_OF_NEXT(prev, l->offset);
    void *prev_prev         = prev;
    prev                    = TO_PTR(*prev_adr_of_next);
    if (prev == item) {
      kassert(prev_prev != item_next);
      *prev_adr_of_next = item_next;
      *addr_next        = nullptr;
      return item;
    }
  }
  return nullptr;
}
