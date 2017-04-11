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

Vec<ProxyAcceptor> ProxyAcceptors;

// Called from InkAPI.cc
NetProcessor::AcceptOptions
make_net_accept_options(const ProxyPort *port, unsigned nthreads)
{
	NetProcessor::AcceptOptions net;
	
	net.accept_threads = nthreads;
	
	if (port) {
    net.ip_family             = port->m_family;
    net.local_port            = port->m_port;

    if (port->m_inbound_ip.isValid()) {
      net.local_ip = port->m_inbound_ip;
    } 
  }
}

static void
MakeProxyAcceptor(ProxyAcceptor &acceptor, ProxyPort &port, unsigned nthreads)
{
	NetProcessor::AcceptOptions &net_opt = acceptor._net_opt;
	ProxySessionAccept::Options accept_opt;

	net_opt = make_net_accept_options(&port, nthreads);

  if (port.m_outbound_ip4.isValid()) {
    accept_opt.outbound_ip4 = port.m_outbound_ip4;
  }

  if (port.m_outbound_ip6.isValid()) {
    accept_opt.outbound_ip6 = port.m_outbound_ip6;
  }
	
	acceptor._accept = new ProxySessionAccept(accept_opt); 
}

void
init_ProxyServer(int n_accept_threads)
{
	ProxyPort::Group &proxy_ports = ProxyPort::global();

	for (int i = 0, n = proxy_ports.length(); i < n; ++i) {
    MakeProxyAcceptor(ProxyAcceptors.add(), proxy_ports[i], n_accept_threads);
  }
}

void
start_ProxyServer()
{
	ProxyPort::Group &proxy_ports = ProxyPort::global();
	
	ink_assert(proxy_ports.length() == ProxyAcceptors.length());
	for (int i = 0, n = proxy_ports.length(); i < n; ++i) {
		ProxyAcceptor &acceptor = ProxyAcceptors[i];
		ProxyPort &port         = proxy_ports[i];
		if (nullptr == netProcessor.main_accept(acceptor._accept, port.m_fd, acceptor._net_opt)) {
      return;	
		}
	}
}
