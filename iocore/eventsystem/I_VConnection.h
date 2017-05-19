//
// Created by 宋辰伟 on 2017/4/19.
//

#ifndef TEST_LOCK_I_VCONNECTION_H
#define TEST_LOCK_I_VCONNECTION_H

#include "I_EventSystem.h"

#define VCONNECTION_CACHE_DATA_BASE 0
#define VCONNECTION_NET_DATA_BASE 100
#define VCONNECTION_API_DATA_BASE 200

#define VC_EVENT_NONE EVENT_NONE

/** When a Continuation is first scheduled on a processor. */
#define VC_EVENT_IMMEDIATE EVENT_IMMEDIATE

#define VC_EVENT_READ_READY VC_EVENT_EVENTS_START

/**
  Any data in the accociated buffer *will be written* when the
  Continuation returns.

*/
#define VC_EVENT_WRITE_READY (VC_EVENT_EVENTS_START + 1)

#define VC_EVENT_READ_COMPLETE (VC_EVENT_EVENTS_START + 2)
#define VC_EVENT_WRITE_COMPLETE (VC_EVENT_EVENTS_START + 3)

/**
  No more data (end of stream). It should be interpreted by a
  protocol engine as either a COMPLETE or ERROR.

*/
#define VC_EVENT_EOS (VC_EVENT_EVENTS_START + 4)

#define VC_EVENT_ERROR EVENT_ERROR

/**
  VC_EVENT_INACTIVITY_TIMEOUT indiates that the operation (read or write) has:
    -# been enabled for more than the inactivity timeout period
       (for a read, there has been space in the buffer)
       (for a write, there has been data in the buffer)
    -# no progress has been made
       (for a read, no data has been read from the connection)
       (for a write, no data has been written to the connection)

*/
#define VC_EVENT_INACTIVITY_TIMEOUT (VC_EVENT_EVENTS_START + 5)

/**
  Total time for some operation has been exeeded, regardless of any
  intermediate progress.

*/
#define VC_EVENT_ACTIVE_TIMEOUT (VC_EVENT_EVENTS_START + 6)

#define VC_EVENT_OOB_COMPLETE (VC_EVENT_EVENTS_START + 7)

//
// Event names
//

//
// VC_EVENT_READ_READ occurs when data *has been written* into
// the associated buffer.
//
// VC_EVENT_ERROR indicates that some error has occured.  The
// "data" will be either 0 if the errno is unavailable or errno.
//
// VC_EVENT_INTERVAL indidates that an interval timer has expired.
//

//
// Event return codes
//
#define VC_EVENT_DONE CONTINUATION_DONE
#define VC_EVENT_CONT CONTINUATION_CONT

  enum ShutdownHowTo_t {
    IO_SHUTDOWN_READ = 0,
    IO_SHUTDOWN_WRITE,
    IO_SHUTDOWN_READWRITE
  };

class VConnection : public Continuation
{
public:
    virtual ~VConnection();
    virtual VIO *do_io_read(Continuation *c = nullptr, int64_t nbytes = INT64_MAX, MIOBuffer *buf = 0) = 0;
    virtual VIO *do_io_write(Continuation *c = nullptr, int64_t nbytes = INT64_MAX, IOBufferReader *buf = 0, bool owner = false) = 0;
    virtual void do_io_close(int lerrno = -1) = 0;
    virtual void do_io_shutdown(ShutdownHowTo_t howto) = 0;
    VConnection(Mutex *aMutex);
    VConnection(std::shared_ptr<Mutex> &aMutex);
    VIO *do_io(int op, Continuation *c = nullptr, int64_t nbytes = INT64_MAX, MIOBuffer *buf = 0, int data = 0);
    virtual void set_continuation(VIO *vio, Continuation *cont);
    virtual void reenable(VIO *vio);
    virtual void reenable_re(VIO *vio);

    virtual bool
    get_data(int id, void *data)
    {
        (void)id;
        (void)data;
        return false;
    }

    virtual bool
    set_data(int id, void *data)
    {
        (void)id;
        (void)data;
        return false;
    }

public:
    int lerrno;
};

#endif //TEST_LOCK_I_VCONNECTION_H
