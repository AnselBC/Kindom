//
// Created by 宋辰伟 on 2017/4/19.
//

#ifndef TEST_LOCK_I_VIO_H
#define TEST_LOCK_I_VIO_H

#include "I_EventSystem.h"

class Continuation;
class VConnection;
class IOVConnection;
class MIOBuffer;
class Mutex;

class VIO
{
public:
    ~VIO() {};
    Continuation *get_continuation();
    void set_continuation(Continuation *cont);

    void done();

    int64_t ntodo();

    void set_writer(MIOBuffer *writer);
    void set_reader(IOBufferReader *reader);
    MIOBuffer *get_writer();
    IOBufferReader *get_reader();

    void reenable();
    void reenable_re();

    VIO(int aop);
    VIO();

    enum {
        NONE = 0,
        READ,
        WRITE,
        CLOSE,
        ABORT,
        SHUTDOWN_READ,
        SHUTDOWN_WRITE,
        SHUTDOWN_READWRITE,
        SEEK,
        PREAD,
        PWRITE,
        STAT,
    };

public:
    Continuation *_cont;
    int64_t nbytes;
    int64_t ndone;
    int op;
    MIOBufferAccessor buffer;
    VConnection *vc_server;
    std::shared_ptr<Mutex> mutex;
};

#endif //TEST_LOCK_I_VIO_H
