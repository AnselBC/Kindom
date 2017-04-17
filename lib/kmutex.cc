//
// Created by 宋辰伟 on 2017/4/17.
//

#include "kmutex.h"

// Define the _g_mattr first to avoid static initialization order fiasco.
x_pthread_mutexattr_t _g_mattr;

kmutex __global_death = PTHREAD_MUTEX_INITIALIZER;
