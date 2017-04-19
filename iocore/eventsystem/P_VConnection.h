//
// Created by 宋辰伟 on 2017/4/19.
//

#ifndef TEST_LOCK_P_VCONNECTION_H
#define TEST_LOCK_P_VCONNECTION_H

#include "I_EventSystem.h"

inline const char *
get_vc_event_name(int event)
{
    switch (event) {
        default:
            return "unknown event";
        case VC_EVENT_NONE:
            return "VC_EVENT_NONE";
        case VC_EVENT_IMMEDIATE:
            return "VC_EVENT_IMMEDIATE";
        case VC_EVENT_READ_READY:
            return "VC_EVENT_READ_READY";
        case VC_EVENT_WRITE_READY:
            return "VC_EVENT_WRITE_READY";
        case VC_EVENT_READ_COMPLETE:
            return "VC_EVENT_READ_COMPLETE";
        case VC_EVENT_WRITE_COMPLETE:
            return "VC_EVENT_WRITE_COMPLETE";
        case VC_EVENT_EOS:
            return "VC_EVENT_EOS";
        case VC_EVENT_ERROR:
            return "VC_EVENT_ERROR";
        case VC_EVENT_INACTIVITY_TIMEOUT:
            return "VC_EVENT_INACTIVITY_TIMEOUT";
        case VC_EVENT_ACTIVE_TIMEOUT:
            return "VC_EVENT_ACTIVE_TIMEOUT";
    }
}

inline
VConnection::VConnection(Mutex *aMutex) : Continuation(aMutex), lerrno(0)
{
    SET_HANDLER(0);
}

inline
VConnection::VConnection(std::shared_ptr<Mutex> &aMutex) : Continuation(aMutex), lerrno(0)
{
    SET_HANDLER(0);
}

inline
VConnection::~VConnection()
{
}


inline VIO *
vc_do_io_write(VConnection *vc, Continuation *cont, int64_t nbytes, MIOBuffer *buf, int64_t offset)
{
    IOBufferReader *reader = buf->alloc_reader();

    if (offset > 0)
        reader->consume(offset);

    return vc->do_io_write(cont, nbytes, reader, true);
}

inline VIO *
VConnection::do_io(int op, Continuation *c, int64_t nbytes, MIOBuffer *cb, int data)
{
    switch (op) {
        case VIO::READ:
            return do_io_read(c, nbytes, cb);
        case VIO::WRITE:
            return vc_do_io_write(this, c, nbytes, cb, data);
        case VIO::CLOSE:
            do_io_close();
            return nullptr;
        case VIO::ABORT:
            do_io_close(data);
            return nullptr;
        case VIO::SHUTDOWN_READ:
            do_io_shutdown(IO_SHUTDOWN_READ);
            return nullptr;
        case VIO::SHUTDOWN_WRITE:
            do_io_shutdown(IO_SHUTDOWN_WRITE);
            return nullptr;
        case VIO::SHUTDOWN_READWRITE:
            do_io_shutdown(IO_SHUTDOWN_READWRITE);
            return nullptr;
    }
    kassert(!"cannot use default implementation for do_io operation");
    return nullptr;
}

inline void
VConnection::set_continuation(VIO *, Continuation *)
{
}
inline void
VConnection::reenable(VIO *)
{
}
inline void
VConnection::reenable_re(VIO *vio)
{
    reenable(vio);
}

#endif //TEST_LOCK_P_VCONNECTION_H
