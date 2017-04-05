//
// Created by 宋辰伟 on 2017/4/2.
//

#include "king/king_error.h"
#include "king/king_defs.h"
#include <assert.h>
#include "stdio.h"
#include "king/king_mutex.h"

x_pthread_mutexattr_t _g_mattr;

kmutex __global_death = PTHREAD_MUTEX_INITIALIZER;
