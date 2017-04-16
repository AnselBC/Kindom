//
// Created by 宋辰伟 on 2017/4/16.
//

#ifndef PROJECT_I_THREAD_H
#define PROJECT_I_THREAD_H

#include "Kindom.h"

using namespace std;

class MUtex;


class Thread {
public:
    // Thread mutex;
    std::shared_ptr<Mutex> mutex;
    // Thread tid;
    kthread  tid;
    // Thread specified data.
    static kthread_key thread_data;

    // Time
    ktime cur_time;

    static void update_time() {};

    static ktime
    get_time() {
        return cur_time;
    }

    void
    set_specific() {
        assert(!pthread_setspecific(thread_data, this));
    }

    Thread *
    this_thread() {
        return (Thread *)pthread_getspecific(thread_data);
    }

    const char *
    get_thread_nameI() {
        return thr_name;
    }

    void start();

    void
    set_thread_name(const char *name) {
        thr_name = name;
    }

private:
    const char thr_name[MAX_THREAD_NAME];

};


#endif //PROJECT_I_THREAD_H
