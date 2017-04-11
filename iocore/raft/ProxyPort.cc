/** @file

  HTTP configuration support.

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

#include "P_Net.h"
#include "ts/Tokenizer.h"
#include "ProxyRaft.h"

const char *const ProxyPort::OPT_IPV6               = "ipv6";
const char *const ProxyPort::OPT_IPV4               = "ipv4";
const char *const ProxyPort::OPT_OUTBOUND_IP_PREFIX = "ip-out";
const char *const ProxyPort::OPT_INBOUND_IP_PREFIX  = "ip-in";

size_t const OPT_OUTBOUND_IP_PREFIX_LEN = strlen(ProxyPort::OPT_OUTBOUND_IP_PREFIX);
size_t const OPT_INBOUND_IP_PREFIX_LEN  = strlen(ProxyPort::OPT_INBOUND_IP_PREFIX);

namespace
{
// Solaris work around. On that OS the compiler will not let me use an
// instantiated instance of Vec<self> inside the class, even if
// static. So we have to declare it elsewhere and then import via
// reference. Might be a problem with Vec<> creating a fixed array
// rather than allocating on first use (compared to std::vector<>).
ProxyPort::Group GLOBAL_DATA;
}

ProxyPort::Group &ProxyPort::m_global = GLOBAL_DATA;

ProxyPort::ProxyPort() : m_fd(ts::NO_FD), m_port(0), m_family(AF_INET)
{
}

bool
ProxyPort::loadValue(Vec<self> &ports, const char *text)
{
  unsigned old_port_length = ports.length(); // remember this.
  if (text && *text) {
    Tokenizer tokens(", ");
    int n_ports = tokens.Initialize(text);
    if (n_ports > 0) {
      for (int p = 0; p < n_ports; ++p) {
        const char *elt = tokens[p];
        ProxyPort entry;
        if (entry.processOptions(elt)) {
          ports.push_back(entry);
        } else {
          Warning("No valid definition was found in proxy port configuration element '%s'", elt);
        }
      }
    }
  }
  return ports.length() > old_port_length; // we added at least one port.
}

bool
ProxyPort::processOptions(const char *opts)
{
  bool zret           = false; // found a port?
  bool af_set_p       = false; // AF explicitly specified?
  bool host_res_set_p = false; // Host resolution order set explicitly?
  bool sp_set_p       = false; // Session protocol set explicitly?
  bool bracket_p      = false; // found an open bracket in the input?
  const char *value;           // Temp holder for value of a prefix option.
  IpAddr ip;                   // temp for loading IP addresses.
  Vec<char *> values;          // Pointers to single option values.

  // Make a copy we can modify safely.
  size_t opts_len = strlen(opts) + 1;
  char *text      = static_cast<char *>(alloca(opts_len));
  memcpy(text, opts, opts_len);

  // Split the copy in to tokens.
  char *token = nullptr;
  for (char *spot = text; *spot; ++spot) {
    if (bracket_p) {
      if (']' == *spot) {
        bracket_p = false;
      }
    } else if (':' == *spot) {
      *spot = 0;
      token = nullptr;
    } else {
      if (!token) {
        token = spot;
        values.push_back(token);
      }
      if ('[' == *spot) {
        bracket_p = true;
      }
    }
  }

  if (bracket_p) {
    Warning("Invalid port descriptor '%s' - left bracket without closing right bracket", opts);
    return zret;
  }

  for (int i = 0, n_items = values.length(); i < n_items; ++i) {
    const char *item = values[i];
    if (isdigit(item[0])) { // leading digit -> port value
      char *ptr;
      int port = strtoul(item, &ptr, 10);
      if (ptr == item) {
        // really, this shouldn't happen, since we checked for a leading digit.
        Warning("Mangled port value '%s' in port configuration '%s'", item, opts);
      } else if (port <= 0 || 65536 <= port) {
        Warning("Port value '%s' out of range (1..65535) in port configuration '%s'", item, opts);
      } else {
        m_port = port;
        zret   = true;
      }
    } else if (0 == strcasecmp(OPT_IPV6, item)) {
      m_family = AF_INET6;
      af_set_p = true;
    } else if (0 == strcasecmp(OPT_IPV4, item)) {
      m_family = AF_INET;
      af_set_p = true;
    } else if (nullptr != (value = this->checkPrefix(item, OPT_INBOUND_IP_PREFIX, OPT_INBOUND_IP_PREFIX_LEN))) {
      if (0 == ip.load(value)) {
        m_inbound_ip = ip;
      } else {
        Warning("Invalid IP address value '%s' in port descriptor '%s'", item, opts);
      }
    } else if (nullptr != (value = this->checkPrefix(item, OPT_OUTBOUND_IP_PREFIX, OPT_OUTBOUND_IP_PREFIX_LEN))) {
      if (0 == ip.load(value)) {
        this->outboundIp(ip.family()) = ip;
      } else {
        Warning("Invalid IP address value '%s' in port descriptor '%s'", item, opts);
      }
    } else {
      // error unknown option
    }
  }

  return zret;
}

const char *
ProxyPort::checkPrefix(const char *src, char const *prefix, size_t prefix_len)
{
  const char *zret = nullptr;
  if (0 == strncasecmp(prefix, src, prefix_len)) {
    src += prefix_len;
    if ('-' == *src || '=' == *src) {
      ++src; // permit optional '-' or '='
    }
    zret = src;
  }
  return zret;
}
