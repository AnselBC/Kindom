//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_KATOMIC_H
#define TEST_LOCK_KATOMIC_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "Kindom.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void kqueue_load_64(void *dst, void *src);

#ifdef __x86_64__
#define KQUEUE_LD64(dst, src) *((uint64_t *)&(dst)) = *((uint64_t *)&(src))
#else
#define KQUEUE_LD64(dst, src) (kqueue_load_64((void *)&(dst), (void *)&(src)))
#endif

#if KHAS_128BIT_CAS
#define KQUEUE_LD(dst, src)                                                          \
  do {                                                                               \
    *(__int128_t *)&(dst) = __sync_val_compare_and_swap((__int128_t *)&(src), 0, 0); \
  } while (0)
#else
#define KQUEUE_LD(dst, src) KQUEUE_LD64(dst, src)
#endif

#ifdef DEBUG
#define FROM_PTR(_x) (void *)(((uintptr_t)_x) + 1)
#define TO_PTR(_x) (void *)(((uintptr_t)_x) - 1)
#else
#define FROM_PTR(_x) ((void *)(_x))
#define TO_PTR(_x) ((void *)(_x))
#endif

typedef union {
#if (defined(__i386__) || defined(__arm__) || defined(__mips__)) && (SIZEOF_VOIDP == 4)
  typedef int32_t version_type;
  typedef int64_t data_type;
#elif KHAS_128BIT_CAS
  typedef int64_t version_type;
  typedef __int128_t data_type;
#else
  typedef int64_t version_type;
  typedef int64_t data_type;
#endif

  struct {
    void *pointer;
    version_type version;
  } s;

  data_type data;
} head_p;

#if (defined(__i386__) || defined(__arm__) || defined(__mips__)) && (SIZEOF_VOIDP == 4)
#define FREELIST_POINTER(_x) (_x).s.pointer
#define FREELIST_VERSION(_x) (_x).s.version
#define SET_FREELIST_POINTER_VERSION(_x, _p, _v) \
  (_x).s.pointer = _p;                           \
  (_x).s.version = _v
#elif KHAS_128BIT_CAS
#define FREELIST_POINTER(_x) (_x).s.pointer
#define FREELIST_VERSION(_x) (_x).s.version
#define SET_FREELIST_POINTER_VERSION(_x, _p, _v) \
  (_x).s.pointer = _p;                           \
  (_x).s.version = _v
#elif defined(__x86_64__) || defined(__ia64__) || defined(__powerpc64__) || defined(__aarch64__)
#define FREELIST_POINTER(_x) \
  ((void *)(((((intptr_t)(_x).data) << 16) >> 16) | (((~((((intptr_t)(_x).data) << 16 >> 63) - 1)) >> 48) << 48))) // sign extend
#define FREELIST_VERSION(_x) (((intptr_t)(_x).data) >> 48)
#define SET_FREELIST_POINTER_VERSION(_x, _p, _v) (_x).data = ((((intptr_t)(_p)) & 0x0000FFFFFFFFFFFFULL) | (((_v)&0xFFFFULL) << 48))
#else
#error "unsupported processor"
#endif

typedef struct {
  volatile head_p head;
  const char *name;
  uint32_t offset;
} KAtomicList;

#if !defined(KQUEUE_NT)
#define KATOMICLIST_EMPTY(_x) (!(TO_PTR(FREELIST_POINTER((_x.head)))))
#else
/* kqueue_nt.c doesn't do the FROM/TO pointer swizzling */
#define KATOMICLIST_EMPTY(_x) (!((FREELIST_POINTER((_x.head)))))
#endif

void katomiclist_init(KAtomicList *l, const char *name, uint32_t offset_to_next);
void *katomiclist_push(KAtomicList *l, void *item);
void *katomiclist_pop(KAtomicList *l);
void *katomiclist_popall(KAtomicList *l);
/*
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * only if only one thread is doing pops it is possible to have a "remove"
 * which only that thread can use as well.
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 */
void *katomiclist_remove(KAtomicList *l, void *item);

inline void
kqueue_load_64(void *dst, void *src)
{
#if (defined(__i386__) || defined(__arm__) || defined(__mips__)) && (SIZEOF_VOIDP == 4)
  volatile int32_t src_version = (*(head_p *)src).s.version;
  void *src_pointer            = (*(head_p *)src).s.pointer;

  (*(head_p *)dst).s.version = src_version;
  (*(head_p *)dst).s.pointer = src_pointer;
#else
  *(void **)dst = *(void **)src;
#endif
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

typedef volatile int8_t vint8;
typedef volatile int16_t vint16;
typedef volatile int32_t vint32;
typedef volatile int64_t vint64;
typedef volatile uint64_t vuint64;
typedef volatile long vlong;
typedef volatile void *vvoidp;

typedef vint8 *pvint8;
typedef vint16 *pvint16;
typedef vint32 *pvint32;
typedef vint64 *pvint64;
typedef vuint64 *pvuint64;
typedef vlong *pvlong;
typedef vvoidp *pvvoidp;

/* GCC compiler >= 4.1 */
#if defined(__GNUC__) && (((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ >= 5))

/* see http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html */

// katomic_swap(ptr, value)
// Writes @value into @ptr, returning the previous value.
template <typename T>
static inline T
katomic_swap(volatile T *mem, T value)
{
  return __sync_lock_test_and_set(mem, value);
}

// katomic_cas(mem, prev, next)
// Atomically store the value @next into the pointer @mem, but only if the current value at @mem is @prev.
// Returns true if @next was successfully stored.
template <typename T>
static inline bool
katomic_cas(volatile T *mem, T prev, T next)
{
  return __sync_bool_compare_and_swap(mem, prev, next);
}

// katomic_increment(ptr, count)
// Increment @ptr by @count, returning the previous value.
template <typename Type, typename Amount>
static inline Type
katomic_increment(volatile Type *mem, Amount count)
{
  return __sync_fetch_and_add(mem, (Type)count);
}

// katomic_decrement(ptr, count)
// Decrement @ptr by @count, returning the previous value.
template <typename Type, typename Amount>
static inline Type
katomic_decrement(volatile Type *mem, Amount count)
{
  return __sync_fetch_and_sub(mem, (Type)count);
}

// Special hacks for ARM 32-bit
#if (defined(__arm__) || defined(__mips__)) && (SIZEOF_VOIDP == 4)
extern kmutex __global_death;

template <>
inline int64_t
katomic_swap<int64_t>(pvint64 mem, int64_t value)
{
  int64_t old;
  kmutex_acquire(&__global_death);
  old  = *mem;
  *mem = value;
  kmutex_release(&__global_death);
  return old;
}

template <>
inline bool
katomic_cas<int64_t>(pvint64 mem, int64_t old, int64_t new_value)
{
  int64_t curr;
  kmutex_acquire(&__global_death);
  curr = *mem;
  if (old == curr)
    *mem = new_value;
  kmutex_release(&__global_death);
  if (old == curr)
    return 1;
  return 0;
}

template <typename Amount>
static inline int64_t
katomic_increment(pvint64 mem, Amount value)
{
  int64_t curr;
  kmutex_acquire(&__global_death);
  curr = *mem;
  *mem = curr + value;
  kmutex_release(&__global_death);
  return curr;
}

template <typename Amount>
static inline int64_t
katomic_decrement(pvint64 mem, Amount value)
{
  int64_t curr;
  kmutex_acquire(&__global_death);
  curr = *mem;
  *mem = curr - value;
  kmutex_release(&__global_death);
  return curr;
}

template <typename Amount>
static inline uint64_t
katomic_increment(pvuint64 mem, Amount value)
{
  uint64_t curr;
  kmutex_acquire(&__global_death);
  curr = *mem;
  *mem = curr + value;
  kmutex_release(&__global_death);
  return curr;
}

template <typename Amount>
static inline uint64_t
katomic_decrement(pvuint64 mem, Amount value)
{
  uint64_t curr;
  kmutex_acquire(&__global_death);
  curr = *mem;
  *mem = curr - value;
  kmutex_release(&__global_death);
  return curr;
}

#endif /* Special hacks for ARM 32-bit */

/* not used for Intel Processors which have sequential(esque) consistency */
#define KWRITE_MEMORY_BARRIER
#define KMEMORY_BARRIER

#else /* not gcc > v4.1.2 */
#error Need a compiler / libc that supports atomic operations, e.g. gcc v4.1.2 or later
#endif

#endif // TEST_LOCK_KATOMIC_H
