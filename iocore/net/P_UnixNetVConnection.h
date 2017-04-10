/** @file

  A brief file description

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

/****************************************************************************


  This file implements an I/O Processor for network I/O on Unix.


 ****************************************************************************/

#ifndef __P_UNIXNETVCONNECTION_H__
#define __P_UNIXNETVCONNECTION_H__

#include "ts/ink_sock.h"
#include "I_NetVConnection.h"

class UnixNetVConnection;

TS_INLINE void
NetVCOptions::reset()
{
  ip_proto  = USE_TCP;
  ip_family = AF_INET;
  local_ip.invalidate();
  local_port         = 0;
  addr_binding       = ANY_ADDR;
  f_blocking         = false;
  f_blocking_connect = false;
  // socks_support      = NORMAL_SOCKS;
  // socks_version      = SOCKS_DEFAULT_VERSION;
  socket_recv_bufsize =
#if defined(RECV_BUF_SIZE)
    RECV_BUF_SIZE;
#else
    0;
#endif
  socket_send_bufsize = 0;
  sockopt_flags       = 0;
  packet_mark         = 0;
  packet_tos          = 0;

  etype = ET_NET;

  sni_servername    = nullptr;
  clientCertificate = nullptr;
}

struct OOB_callback : public Continuation {
  char *data;
  int length;
  Event *trigger;
  UnixNetVConnection *server_vc;
  Continuation *server_cont;
  int retry_OOB_send(int, Event *);

  OOB_callback(Ptr<ProxyMutex> &m, NetVConnection *vc, Continuation *cont, char *buf, int len)
    : Continuation(m), data(buf), length(len), trigger(0)
  {
    server_vc   = (UnixNetVConnection *)vc;
    server_cont = cont;
    SET_HANDLER(&OOB_callback::retry_OOB_send);
  }
};

TS_INLINE void
NetVCOptions::set_sock_param(int _recv_bufsize, int _send_bufsize, unsigned long _opt_flags, unsigned long _packet_mark,
                             unsigned long _packet_tos)
{
  socket_recv_bufsize = _recv_bufsize;
  socket_send_bufsize = _send_bufsize;
  sockopt_flags       = _opt_flags;
  packet_mark         = _packet_mark;
  packet_tos          = _packet_tos;
}

class UnixNetVConnection : public NetVConnection
{
public:
  virtual int64_t outstanding();
  virtual VIO *do_io_read(Continuation *c, int64_t nbytes, MIOBuffer *buf);
  virtual VIO *do_io_write(Continuation *c, int64_t nbytes, IOBufferReader *buf, bool owner = false);

  virtual bool get_data(int id, void *data);

  virtual Action *send_OOB(Continuation *cont, char *buf, int len);
  virtual void cancel_OOB();

  virtual void do_io_close(int lerrno = -1);
  virtual void do_io_shutdown(ShutdownHowTo_t howto);

  ////////////////////////////////////////////////////////////
  // Set the timeouts associated with this connection.      //
  // active_timeout is for the total elasped time of        //
  // the connection.                                        //
  // inactivity_timeout is the elapsed time from the time   //
  // a read or a write was scheduled during which the       //
  // connection  was unable to sink/provide data.           //
  // calling these functions repeatedly resets the timeout. //
  // These functions are NOT THREAD-SAFE, and may only be   //
  // called when handing an  event from this NetVConnection,//
  // or the NetVConnection creation callback.               //
  ////////////////////////////////////////////////////////////
  virtual void set_active_timeout(ink_hrtime timeout_in);
  virtual void set_inactivity_timeout(ink_hrtime timeout_in);
  virtual void cancel_active_timeout();
  virtual void cancel_inactivity_timeout();
  virtual void set_action(Continuation *c);
  virtual void add_to_keep_alive_queue();
  virtual void remove_from_keep_alive_queue();
  virtual bool add_to_active_queue();
  virtual void remove_from_active_queue();

  // The public interface is VIO::reenable()
  virtual void reenable(VIO *vio);
  virtual void reenable_re(VIO *vio);

  virtual SOCKET get_socket();

  virtual ~UnixNetVConnection();

  UnixNetVConnection();

  int
  populate_protocol(const char **results, int n) const
  {
    int retval = 0;
    return retval;
  }

  const char *
  protocol_contains(const char *tag) const
  {
    const char *retval   = nullptr;
    return retval;
  }

private:
  UnixNetVConnection(const NetVConnection &);
  UnixNetVConnection &operator=(const NetVConnection &);

public:
  /////////////////////////
  // UNIX implementation //
  /////////////////////////
  void set_enabled(VIO *vio);

//  void get_local_sa();

  int startEvent(int event, Event *e);
	virtual int connectUp(EThread *t, int fd);

  virtual ink_hrtime get_inactivity_timeout();
  virtual ink_hrtime get_active_timeout();

  virtual void set_local_addr();
  virtual void set_remote_addr();
  virtual int set_tcp_init_cwnd(int init_cwnd);
  virtual int set_tcp_congestion_control(int side);
  virtual void apply_options();

//  virtual int populate(Connection &con, Continuation *c, void *arg);
	virtual void free(EThread *t);
};

typedef int (UnixNetVConnection::*NetVConnHandler)(int, void *);

TS_INLINE void
UnixNetVConnection::set_remote_addr()
{
 // ats_ip_copy(&remote_addr, &con.addr);
}

TS_INLINE void
UnixNetVConnection::set_local_addr()
{
  // int local_sa_size = sizeof(local_addr);
  // safe_getsockname(con.fd, &local_addr.sa, &local_sa_size);
}

TS_INLINE ink_hrtime
UnixNetVConnection::get_active_timeout()
{
  // return active_timeout_in;
	return 0;
}

TS_INLINE ink_hrtime
UnixNetVConnection::get_inactivity_timeout()
{
  // return inactivity_timeout_in;
	return 0;
}

TS_INLINE void
UnixNetVConnection::set_active_timeout(ink_hrtime timeout_in)
{
}

TS_INLINE void
UnixNetVConnection::set_inactivity_timeout(ink_hrtime timeout_in)
{
}

TS_INLINE void
UnixNetVConnection::cancel_active_timeout()
{
}

TS_INLINE void
UnixNetVConnection::cancel_inactivity_timeout()
{
}

TS_INLINE void
UnixNetVConnection::set_action(Continuation *c)
{
//  action_ = c;
}

TS_INLINE UnixNetVConnection::~UnixNetVConnection()
{
}

TS_INLINE int
UnixNetVConnection::set_tcp_init_cwnd(int init_cwnd)
{
#ifdef TCP_INIT_CWND
  int rv;
  uint32_t val = init_cwnd;
  rv           = setsockopt(con.fd, IPPROTO_TCP, TCP_INIT_CWND, &val, sizeof(val));
  Debug("socket", "Setting TCP initial congestion window (%d) -> %d", init_cwnd, rv);
  return rv;
#else
  Debug("socket", "Setting TCP initial congestion window %d -> unsupported", init_cwnd);
  return -1;
#endif
}

TS_INLINE int
UnixNetVConnection::set_tcp_congestion_control(int side)
{
#ifdef TCP_CONGESTION
/*  RecString congestion_control;
  int ret;

  if (side == CLIENT_SIDE) {
    ret = REC_ReadConfigStringAlloc(congestion_control, "proxy.config.net.tcp_congestion_control_in");
  } else {
    ret = REC_ReadConfigStringAlloc(congestion_control, "proxy.config.net.tcp_congestion_control_out");
  }

  if (ret == REC_ERR_OKAY) {
    int len = strlen(congestion_control);
    if (len > 0) {
      int rv = 0;
      rv     = setsockopt(con.fd, IPPROTO_TCP, TCP_CONGESTION, reinterpret_cast<void *>(congestion_control), len);
      if (rv < 0) {
        Error("Unable to set TCP congestion control on socket %d to \"%.*s\", errno=%d (%s)", con.fd, len, congestion_control,
              errno, strerror(errno));
      } else {
        Debug("socket", "Setting TCP congestion control on socket [%d] to \"%.*s\" -> %d", con.fd, len, congestion_control, rv);
      }
    }
    ats_free(congestion_control);
    return 0;
  }*/
  return -1;
#else
  Debug("socket", "Setting TCP congestion control is not supported on this platform.");
  return -1;
#endif
}

TS_INLINE SOCKET
UnixNetVConnection::get_socket()
{
  // return con.fd;
	return 0;
}

#endif