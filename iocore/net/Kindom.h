//
// Created by 宋辰伟 on 2017/4/16.
//

#ifndef PROJECT_KINDOM_H
#define PROJECT_KINDOM_H

#include <stdio.h>
#include <stdlib.h>

#define Debug 1

// mem interface
#define kmalloc malloc
#define kfree free


// Log interface
#define Debug(_f, ...)   \
    do {                 \
    } while(0)


#define Warn(_f, ...)    \
    do {                 \
    } while(0)

#define Error(_f, ...)   \
    do {                 \
    } while(0)

#define Fatal(_f, ...)   \
    do {                 \
       exit(0)           \
    } while(0)

#define kassert aseert

// Thread interface
#define MAX_THREAD_NAME 30
typedef pthread_t kthread;
typedef pthread_cond_t kcond;
typedef pthread_key_t kthread_key;

// time interface
typedef ktime int64_t;

#endif //PROJECT_KINDOM_H
