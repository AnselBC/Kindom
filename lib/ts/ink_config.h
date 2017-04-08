/** @file

  Some small general interest definitions. The general standard is to
  prefix these defines with TS_.

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

#ifndef _ink_config_h
#define _ink_config_h

// Note: All "defines" should be prefixed with TS_ when appropriate, please
// don't use ATS_ any more.

// Note 2: If you make changes here, please update the traffic_layout.cc file as well.

/* GENERATED FILE WARNING!  DO NOT EDIT ink_config.h
 *
 * You must modify ink_config.h.in instead.
 *
 */

/* Include automake generated defines
 */
#include "ink_autoconf.h"

// Helper macro to convert integer defines into string literals
#define _TS_STR(x) #x
#define TS_STR(x) _TS_STR(x)

/* clang-format off */
#define BUILD_MACHINE "14"
#define BUILD_PERSON "root"
#define BUILD_GROUP "root"
#define BUILD_NUMBER ""

/* Libraries */
#define TS_HAS_JEMALLOC 0
#define TS_HAS_TCMALLOC 0

/* Features */
#define TS_HAS_IN6_IS_ADDR_UNSPECIFIED 1
#define TS_HAS_BACKTRACE 1
#define TS_HAS_PROFILER 0
#define TS_USE_FAST_SDK 0
#define TS_USE_DIAGS 1
#define TS_USE_EPOLL 1
#define TS_USE_KQUEUE 0
#define TS_USE_PORT 0
#define TS_USE_POSIX_CAP 0
#define TS_USE_TPROXY 0
#define TS_HAS_SO_MARK 1
#define TS_HAS_IP_TOS 1
#define TS_USE_HWLOC 0
#define TS_USE_TLS_NPN 1
#define TS_USE_TLS_ALPN 0
#define TS_USE_TLS_SNI 1
#define TS_USE_CERT_CB 0
#define TS_USE_SET_RBIO 0
#define TS_USE_GET_DH_2048_256 0
#define TS_USE_TLS_ECKEY 1
#define TS_USE_LINUX_NATIVE_AIO 0
#define TS_USE_REMOTE_UNWINDING 0
#define TS_USE_SSLV3_CLIENT 0

#define TS_HAS_SO_PEERCRED 1

/* OS API definitions */
#define TS_IP_TRANSPARENT 0
#define TS_HAS_128BIT_CAS 1

/* API */
#define TS_HAS_TESTS 1
#define TS_HAS_WCCP 0

#define TS_MAX_THREADS_IN_EACH_THREAD_TYPE 3072
#define TS_MAX_NUMBER_EVENT_THREADS 4096

#define TS_MAX_HOST_NAME_LEN 256

#define TS_MAX_API_STATS 512

#define SPLIT_DNS 1
#define HTTP_CACHE 1

/* Defaults for user / group */
#define TS_PKGSYSUSER "www-data"
#define TS_PKGSYSGROUP "www-data"

/* Various "build" defines */
#define TS_BUILD_PREFIX "/opt/fusion/cdn/trafficserver"
#define TS_BUILD_EXEC_PREFIX ""
#define TS_BUILD_BINDIR "bin"
#define TS_BUILD_SBINDIR "bin"
#define TS_BUILD_SYSCONFDIR "etc/trafficserver"
#define TS_BUILD_DATADIR "share/trafficserver"
#define TS_BUILD_INCLUDEDIR "include"
#define TS_BUILD_LIBDIR "lib"
#define TS_BUILD_LIBEXECDIR "libexec/trafficserver"
#define TS_BUILD_LOCALSTATEDIR "var"
#define TS_BUILD_RUNTIMEDIR "var/trafficserver"
#define TS_BUILD_LOGDIR "var/log/trafficserver"
#define TS_BUILD_MANDIR "share/man"
#define TS_BUILD_CACHEDIR "var/trafficserver"
#define TS_BUILD_INFODIR "info"

#define TS_BUILD_CANONICAL_HOST "x86_64-unknown-linux-gnu"

#define TS_BUILD_DEFAULT_LOOPBACK_IFACE "lo"
/* clang-format on */

#endif /* _ink_config_h */
