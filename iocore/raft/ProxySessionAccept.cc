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

#include "ProxyRaft.h"

bool
ProxySessionAccept::accept(NetVConnection *netvc, MIOBuffer *iobuf, IOBufferReader *reader)
{
  /*  sockaddr const *client_ip   = netvc->get_remote_addr();
    const AclRecord *acl_record = nullptr;
    ip_port_text_buffer ipb;

    // The backdoor port is now only bound to "localhost", so no
    // reason to check for if it's incoming from "localhost" or not.
    if (backdoor) {
      acl_record = IpAllow::AllMethodAcl();
    } else {
      acl_record = testIpAllowPolicy(client_ip);
      if (!acl_record) {
        ////////////////////////////////////////////////////
        // if client address forbidden, close immediately //
        ////////////////////////////////////////////////////
        Warning("client '%s' prohibited by ip-allow policy", ats_ip_ntop(client_ip, ipb, sizeof(ipb)));
        return false;
      }
    }

    // Set the transport type if not already set
    if (HttpProxyPort::TRANSPORT_NONE == netvc->attributes) {
      netvc->attributes = transport_type;
    }

    if (is_debug_tag_set("http_seq")) {
      Debug("http_seq", "[ProxySessionAccept:mainEvent %p] accepted connection from %s transport type = %d", netvc,
            ats_ip_nptop(client_ip, ipb, sizeof(ipb)), netvc->attributes);
    }

    Http1ClientSession *new_session = THREAD_ALLOC_INIT(http1ClientSessionAllocator, this_ethread());

    // copy over session related data.
    new_session->f_outbound_transparent    = f_outbound_transparent;
    new_session->f_transparent_passthrough = f_transparent_passthrough;
    new_session->outbound_ip4              = outbound_ip4;
    new_session->outbound_ip6              = outbound_ip6;
    new_session->outbound_port             = outbound_port;
    new_session->host_res_style            = ats_host_res_from(client_ip->sa_family, host_res_preference);
    new_session->acl_record                = acl_record;

    new_session->new_connection(netvc, iobuf, reader, backdoor);
  */
  return true;
}

int
ProxySessionAccept::mainEvent(int event, void *data)
{
  NetVConnection *netvc;
  ink_release_assert(event == NET_EVENT_ACCEPT || event == EVENT_ERROR);
  ink_release_assert((event == NET_EVENT_ACCEPT) ? (data != nullptr) : (1));

  if (event == NET_EVENT_ACCEPT) {
    VIO *vio;
    netvc                       = static_cast<NetVConnection *>(data);
    ProxyClientSession *session = new ProxyClientSession(nullptr, nullptr, netvc);
    session->outbound_ip4       = outbound_ip4;
    session->outbound_ip6       = outbound_ip6;
    session->outbound_port      = outbound_port;

    if (!session->reader->is_read_avail_more_than(0)) {
      vio = netvc->do_io_read(session, BUFFER_SIZE_FOR_INDEX(ProxyClientSession::buffer_size_index), session->iobuf);
      vio->reenable();
    } else {
      vio = netvc->do_io_read(nullptr, 0, nullptr);
      session->accept(event, data);
    }

    return EVENT_CONT;
  }

  ink_abort("HTTP accept received fatal error: errno = %d", -((int)(intptr_t)data));
  return EVENT_CONT;
}
