//
// Created by 宋辰伟 on 2017/4/19.
//

#ifndef TEST_LOCK_P_VIO_H
#define TEST_LOCK_P_VIO_H

#include "I_VIO.h"

inline
VIO::VIO(int aop) : _cont(nullptr), nbytes(0), ndone(0), op(aop), buffer(), vc_server(0), mutex(0)
{
}

inline
VIO::VIO() : _cont(0), nbytes(0), ndone(0), op(VIO::NONE), buffer(), vc_server(0), mutex(0)
{
}

inline Continuation *
VIO::get_continuation()
{
    return _cont;
}

inline void
VIO::set_writer(MIOBuffer *writer)
{
    buffer.writer_for(writer);
}

inline void
VIO::set_reader(IOBufferReader *reader)
{
    buffer.reader_for(reader);
}

inline MIOBuffer *
VIO::get_writer()
{
    return buffer.writer();
}

inline IOBufferReader *
VIO::get_reader()
{
    return (buffer.reader());
}

inline int64_t
VIO::ntodo()
{
    return nbytes - ndone;
}

inline void
VIO::done()
{
    if (buffer.reader())
        nbytes = ndone + buffer.reader()->read_avail();
    else
        nbytes = ndone;
}

inline void
VIO::set_continuation(Continuation *acont)
{
    if (vc_server)
        vc_server->set_continuation(this, acont);
    if (acont) {
        mutex = acont->mutex;
        _cont = acont;
    } else {
        mutex = nullptr;
        _cont = nullptr;
    }
    return;
}

inline void
VIO::reenable()
{
    if (vc_server)
        vc_server->reenable(this);
}

inline void
VIO::reenable_re()
{
    if (vc_server)
        vc_server->reenable_re(this);
}

#endif //TEST_LOCK_P_VIO_H
