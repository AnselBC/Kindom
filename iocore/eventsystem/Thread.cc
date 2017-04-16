//
// Created by 宋辰伟 on 2017/4/16.
//

#include "EventSystem.h"
#include "Kindom.h"
static kthread_key init_thread_key();

ktime Thread::cur_time = 0;
kthread Thread::thread_data = init_thread_key();

static kthread
init_thread_key()
{
    kassert(!pthread_key_create(&Thread::thread_data_key, nullptr));
    return Thread::thread_data_key;
}