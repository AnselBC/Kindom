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

#ifndef _PROXYPORT_H_
#define _PROXYPORT_H_

#include "P_Net.h"
#include "ts/Vec.h"

struct ProxyAcceptor {
  /// Accept continuation.
  Continuation *_accept;
  /// Options for @c NetProcessor.
  NetProcessor::AcceptOptions _net_opt;

  /// Default constructor.
  ProxyAcceptor() : _accept(nullptr) {}
};

struct ProxyPort {
private:
  typedef ProxyPort self; ///< Self reference type.
public:
  /// Explicitly supported collection of proxy ports.
  typedef Vec<self> Group;

  /// Type of transport on the connection.

  int m_fd;         ///< Pre-opened file descriptor if present.
  in_port_t m_port; ///< Port on which to listen.
  uint8_t m_family; ///< IP address family.
  IpAddr m_inbound_ip;
  IpAddr m_outbound_ip4;
  IpAddr m_outbound_ip6;

  ProxyPort();

  /** Select the local outbound address object.

      @return The IP address for @a family
  */
  IpAddr &outboundIp(uint16_t family ///< IP address family.
                     );

  /// Process options text.
  /// @a opts should not contain any whitespace, only the option string.
  /// This object's internal state is updated as specified by @a opts.
  /// @return @c true if a port option was successfully processed, @c false otherwise.
  bool processOptions(const char *opts ///< String containing the options.
                      );

  /** Global instance.

      This is provided because most of the work with this data is used as a singleton
      and it's handy to encapsulate it here.
  */
  static Vec<self> &global();

  /// Check for SSL ports.
  /// @return @c true if any port in @a ports is an SSL port.
  /// static bool hasSSL(Group const &ports ///< Ports to check.
  ///                   );

  /// Check for SSL ports.
  /// @return @c true if any global port is an SSL port.
  /// static bool hasSSL();

  /** Load all relevant configuration data.

      This is hardwired to look up the appropriate values in the
      configuration files. It clears @a ports and then loads all found
      values in to it.

      @return @c true if at least one valid port description was
      found, @c false if none.
  */
  static bool loadConfig(Vec<self> &ports ///< Destination for found port data.
                         );

  /** Load all relevant configuration data into the global ports.

      @return @c true if at least one valid port description was
      found, @c false if none.
  */
  // static bool loadConfig();

  /** Load ports from a value string.

      Load ports from single string with port descriptors. Ports
      found are added to @a ports. @a value may safely be @c nullptr or empty.

      @note This is used primarily internally but is available if needed.
      @return @c true if a valid port was found, @c false if none.
  */
  static bool loadValue(Vec<self> &ports, ///< Destination for found port data.
                        const char *value ///< Source port data.
                        );

  /** Load ports from a value string into the global ports.

      Load ports from single string of port descriptors into the
      global set of ports. @a value may safely be @c nullptr or empty.

      @return @c true if a valid port was found, @c false if none.
  */
  static bool loadValue(const char *value ///< Source port data.
                        );

  /// Load default value if @a ports is empty.
  /// @return @c true if the default was needed / loaded.
  static bool loadDefaultIfEmpty(Vec<self> &ports ///< Load target.
                                 );

  /// Load default value into the global set if it is empty.
  /// @return @c true if the default was needed / loaded.
  // static bool loadDefaultIfEmpty();

  /** Find an HTTP port in @a ports.
      If @a family is specified then only ports for that family
      are checked.
      @return The port if found, @c nullptr if not.
  */
  // static self *findHttp(Group const &ports,         ///< Group to search.
  //                      uint16_t family = AF_UNSPEC ///< Desired address family.
  //                      );

  /** Find an HTTP port in the global ports.
      If @a family is specified then only ports for that family
      are checked.
      @return The port if found, @c nullptr if not.
  */
  // static self *findHttp(uint16_t family = AF_UNSPEC);

  /** Create text description to be used for inter-process access.
      Prints the file descriptor and then any options.

      @return The number of characters used for the description.
  */
  int print(char *out, ///< Output string.
            size_t n   ///< Maximum output length.
            );

  static const char *const PORTS_CONFIG_NAME; ///< New unified port descriptor.

  /// Default value if no other values can be found.
  static const char *const DEFAULT_VALUE;

  // Keywords (lower case versions, but compares should be case insensitive)
  static const char *const OPT_FD_PREFIX;               ///< Prefix for file descriptor value.
  static const char *const OPT_OUTBOUND_IP_PREFIX;      ///< Prefix for inbound IP address.
  static const char *const OPT_INBOUND_IP_PREFIX;       ///< Prefix for outbound IP address.
  static const char *const OPT_IPV6;                    ///< IPv6.
  static const char *const OPT_IPV4;                    ///< IPv4
  static const char *const OPT_TRANSPARENT_INBOUND;     ///< Inbound transparent.
  static const char *const OPT_TRANSPARENT_OUTBOUND;    ///< Outbound transparent.
  static const char *const OPT_TRANSPARENT_FULL;        ///< Full transparency.
  static const char *const OPT_TRANSPARENT_PASSTHROUGH; ///< Pass-through non-HTTP.
  static const char *const OPT_SSL;                     ///< SSL (experimental)
  static const char *const OPT_PLUGIN;                  ///< Protocol Plugin handle (experimental)
  static const char *const OPT_BLIND_TUNNEL;            ///< Blind tunnel.
  static const char *const OPT_COMPRESSED;              ///< Compressed.
  static const char *const OPT_HOST_RES_PREFIX;         ///< Set DNS family preference.
  static const char *const OPT_PROTO_PREFIX;            ///< Transport layer protocols.

  static Vec<self> &m_global; ///< Global ("default") data.

protected:
  /** Check a prefix option and find the value.
      @return The address of the start of the value, or @c nullptr if the prefix doesn't match.
  */

  const char *checkPrefix(char const *src ///< Input text
                          ,
                          const char *prefix ///< Keyword prefix
                          ,
                          size_t prefix_len ///< Length of keyword prefix.
                          );
};

inline IpAddr &
ProxyPort::outboundIp(uint16_t family)
{
  static IpAddr invalid; // dummy to make compiler happy about return.
  if (AF_INET == family)
    return m_outbound_ip4;
  else if (AF_INET6 == family)
    return m_outbound_ip6;
  ink_release_assert(!"Invalid family for outbound address on proxy port.");
  return invalid; // never happens but compiler insists.
}

inline Vec<ProxyPort> &
ProxyPort::global()
{
  return m_global;
}

inline bool
ProxyPort::loadValue(const char *value)
{
  return self::loadValue(m_global, value);
}

#endif
