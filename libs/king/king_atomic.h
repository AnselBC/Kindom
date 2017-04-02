//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef PROJECT_KING_ATOMIC_H
#define PROJECT_KING_ATOMIC_H


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "king/king_defs.h"
#include "king/king_apidefs.h"
#include "king/king_mutex.h"

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

// ink_atomic_swap(ptr, value)
// Writes @value into @ptr, returning the previous value.
template <typename T>
static inline T
katomic_swap(volatile T *mem, T value)
{
    return __sync_lock_test_and_set(mem, value);
}

// ink_atomic_cas(mem, prev, next)
// Atomically store the value @next into the pointer @mem, but only if the current value at @mem is @prev.
// Returns true if @next was successfully stored.
template <typename T>
static inline bool
katomic_cas(volatile T *mem, T prev, T next)
{
    return __sync_bool_compare_and_swap(mem, prev, next);
}

// ink_atomic_increment(ptr, count)
// Increment @ptr by @count, returning the previous value.
template <typename Type, typename Amount>
static inline Type
katomic_increment(volatile Type *mem, Amount count)
{
    return __sync_fetch_and_add(mem, (Type)count);
}

// ink_atomic_decrement(ptr, count)
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
  ink_mutex_acquire(&__global_death);
  old  = *mem;
  *mem = value;
  ink_mutex_release(&__global_death);
  return old;
}

template <>
inline bool
katomic_cas<int64_t>(pvint64 mem, int64_t old, int64_t new_value)
{
  int64_t curr;
  ink_mutex_acquire(&__global_death);
  curr = *mem;
  if (old == curr)
    *mem = new_value;
  ink_mutex_release(&__global_death);
  if (old == curr)
    return 1;
  return 0;
}

template <typename Amount>
static inline int64_t
katomic_increment(pvint64 mem, Amount value)
{
  int64_t curr;
  ink_mutex_acquire(&__global_death);
  curr = *mem;
  *mem = curr + value;
  ink_mutex_release(&__global_death);
  return curr;
}

template <typename Amount>
static inline int64_t
katomic_decrement(pvint64 mem, Amount value)
{
  int64_t curr;
  ink_mutex_acquire(&__global_death);
  curr = *mem;
  *mem = curr - value;
  ink_mutex_release(&__global_death);
  return curr;
}

template <typename Amount>
static inline uint64_t
katomic_increment(pvuint64 mem, Amount value)
{
  uint64_t curr;
  ink_mutex_acquire(&__global_death);
  curr = *mem;
  *mem = curr + value;
  ink_mutex_release(&__global_death);
  return curr;
}

template <typename Amount>
static inline uint64_t
katomic_decrement(pvuint64 mem, Amount value)
{
  uint64_t curr;
  ink_mutex_acquire(&__global_death);
  curr = *mem;
  *mem = curr - value;
  ink_mutex_release(&__global_death);
  return curr;
}

#endif /* Special hacks for ARM 32-bit */

/* not used for Intel Processors which have sequential(esque) consistency */
#define KWRITE_MEMORY_BARRIER
#define KMEMORY_BARRIER

#else /* not gcc > v4.1.2 */
#error Need a compiler / libc that supports atomic operations, e.g. gcc v4.1.2 or later
#endif


#endif //PROJECT_KING_ATOMIC_H
