/** @file

  Net subsystem

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

  @section details Details

  Net subsystem is a layer on top the operations sytem network apis. It
  provides an interface for accepting/creating new connection oriented
  (TCP) and connection less (UDP) connetions and for reading/writing
  data through these. The net system can manage 1000s of connections
  very efficiently. Another advantage of using the net system is that
  the SMs dont have be concerned about differences in the net apis of
  various operations systems.

  SMs use the netProcessor global object of the Net System to create new
  connections or to accept incoming connections. When a new connection
  is created the SM gets a NetVConnection which is a handle for the
  underlying connections. The SM can then use the NetVConnection to get
  properties of the connection, read and write data. Net system also
  has socks and ssl support.

 */
#ifndef __I_TESTNET_H_
#define __I_TESTNET_H_

#include "ts/I_Version.h"
// #include "I_EventSystem.h"
#include "P_EventSystem.h"
#include <netinet/in.h>
#include "I_Net.h"


#include "I_NetVConnection.h"
#include "I_NetProcessor.h"
#include "P_UnixNetVConnection.h"

void ink_net_init(ModuleVersion version);
#endif
