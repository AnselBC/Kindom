//
// Created by 宋辰伟 on 2017/4/11.
//

#ifndef PROJECT_PROXYSESSION_H
#define PROJECT_PROXYSESSION_H

#include "ProxyRaft.h"

class ProxyClientSession : public VConnection
{
public:
  ProxyClientSession(MIOBuffer *buffer, IOBufferReader *reader, NetVConnection *vc);

  void new_connection(NetVConnection *new_vc, MIOBuffer *iobuf, IOBufferReader *reader, bool backdoor);
  int accept(int event, void *edata);

  // Implement VConnection interface.
  virtual VIO *
  do_io_read(Continuation *c, int64_t nbytes = INT64_MAX, MIOBuffer *buf = 0)
  {
  }
  virtual VIO *
  do_io_write(Continuation *c = NULL, int64_t nbytes = INT64_MAX, IOBufferReader *buf = 0, bool owner = false)
  {
  }

  virtual void
  do_io_close(int lerrno = -1)
  {
  }
  virtual void
  do_io_shutdown(ShutdownHowTo_t howto)
  {
  }
  virtual void
  reenable(VIO *vio)
  {
  }

  virtual NetVConnection *
  get_netvc() const
  {
    return client_vc;
  }

  virtual void
  release_netvc()
  {
    // Make sure the vio's are also released to avoid
    // later surprises in inactivity timeout
    if (client_vc) {
      client_vc->do_io_read(NULL, 0, NULL);
      client_vc->do_io_write(NULL, 0, NULL);
      client_vc->set_action(NULL);
      client_vc = NULL;
    }
  }

  MIOBuffer *iobuf;
  IOBufferReader *reader;

  static const unsigned buffer_size_index = CLIENT_CONNECTION_FIRST_READ_BUFFER_SIZE_INDEX;

private:
  NetVConnection *client_vc;
  bool tcp_init_cwnd_set;
  bool half_close;
};

TS_INLINE
ProxyClientSession::ProxyClientSession(MIOBuffer *buffer, IOBufferReader *reader, NetVConnection *vc) : VConnection(vc->mutex)
{
  ink_release_assert(vc->mutex != NULL && vc->mutex == this->mutex);
  this->client_vc = vc;
  this->iobuf     = buffer ? buffer : new_MIOBuffer(buffer_size_index);
  this->reader    = reader ? reader : iobuf->alloc_reader();
  SET_HANDLER(&ProxyClientSession::accept);
}

#endif // PROJECT_PROXYSESSION_H
