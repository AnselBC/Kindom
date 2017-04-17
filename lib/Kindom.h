//
// Created by 宋辰伟 on 2017/4/16.
//

#ifndef PROJECT_KINDOM_H
#define PROJECT_KINDOM_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

// #include "List.h"

#define DEBUG 1

// mem interface
#define kmalloc malloc
#define kfree free

// Log interface
#define Debug(_f, ...) \
  do {                 \
  } while (0)

#define Warn(_f, ...) \
  do {                \
  } while (0)

#define Error(_f, ...) \
  do {                 \
  } while (0)

#define Fatal(_f, ...) \
  do {                 \
    assert(!(_f));     \
  } while (0)

#define kassert assert

// Thread interface
#define MAX_THREAD_NAME 30
// typedef pthread_t kthread;
// typedef pthread_cond_t kcond;
// typedef pthread_key_t kthread_key;

// time interface
typedef int64_t ktime;

#endif // PROJECT_KINDOM_H
