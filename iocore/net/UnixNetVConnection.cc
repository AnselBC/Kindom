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

#include "I_TestNet.h"
#include "P_Net.h"

int
UnixNetVConnection::startEvent(int /* event ATS_UNUSED */, Event *e)
{
}

UnixNetVConnection::UnixNetVConnection()
{
  SET_HANDLER((NetVConnHandler)&UnixNetVConnection::startEvent);
}

void
UnixNetVConnection::apply_options()
{
  // con.apply_options(options);
}

VIO *
UnixNetVConnection::do_io_read(Continuation *c, int64_t nbytes, MIOBuffer *buf)
{
	return nullptr;
}

VIO *
UnixNetVConnection::do_io_write(Continuation *c, int64_t nbytes, IOBufferReader *reader, bool owner)
{
	return nullptr;
}

void
UnixNetVConnection::do_io_shutdown(ShutdownHowTo_t howto)
{
}

void
UnixNetVConnection::do_io_close(int lerrno)
{
}

int64_t
UnixNetVConnection::outstanding()
{
  int n;
  int ret = ioctl(this->get_socket(), TIOCOUTQ, &n);
  // if there was an error (such as ioctl doesn't support this call on this platform) then
  // we return -1
  if (ret == -1) {
    return ret;
  }
  return n;
}

bool
UnixNetVConnection::get_data(int id, void *data)
{
  union {
    TSVIO *vio;
    void *data;
    int *n;
  } ptr;

  ptr.data = data;

  switch (id) {
  case TS_API_DATA_READ_VIO:
//    *ptr.vio = (TSVIO) & this->read.vio;
    return true;
  case TS_API_DATA_WRITE_VIO:
//    *ptr.vio = (TSVIO) & this->write.vio;
    return true;
  case TS_API_DATA_CLOSED:
//    *ptr.n = this->closed;
    return true;
  default:
    return false;
  }
}

Action *
UnixNetVConnection::send_OOB(Continuation *cont, char *buf, int len)
{
	return nullptr;
}

void
UnixNetVConnection::cancel_OOB()
{
}

void
UnixNetVConnection::add_to_keep_alive_queue()
{
}

void
UnixNetVConnection::remove_from_keep_alive_queue()
{
}

bool
UnixNetVConnection::add_to_active_queue()
{
	return true;
}

void
UnixNetVConnection::remove_from_active_queue()
{
}

void
UnixNetVConnection::reenable(VIO *vio)
{
}

void
UnixNetVConnection::reenable_re(VIO *vio)
{
}

int
OOB_callback::retry_OOB_send(int /* event ATS_UNUSED */, Event * /* e ATS_UNUSED */)
{
	return EVENT_DONE;
}

void
UnixNetVConnection::set_enabled(VIO *vio)
{
}

void
UnixNetVConnection::free(EThread *t)
{
}

/*int
UnixNetVConnection::populate(Connection &con_in, Continuation *c, void *arg)
{
	return EVENT_DONE;
}*/

int
UnixNetVConnection::connectUp(EThread *t, int fd)
{
	return CONNECT_SUCCESS;
}
