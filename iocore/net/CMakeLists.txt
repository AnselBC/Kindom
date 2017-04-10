PROJECT(test_lock)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lpcre -g -O0 -Wno-invalid-offsetof ")
SET(CMAKE_SOURCE_DIR .)


INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/records
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts 
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ 
${CMAKE_CURRENT_SOURCE_DIR}
) 

Message(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts)

FIND_LIBRARY(RUNTIME_LIB 
/usr/lib  
/usr/local/lib 
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts/
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/records/
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/mgmt/
)

link_directories(
${CMAKE_CURRENT_SOURCE_DIR}/../eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/records
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/mgmt/
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts 
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ 
${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(tsnet SHARED 
#BIO_fastopen.cc
Connection.cc
Inline.cc
Net.cc
NetVConnection.cc
#NetVCTest.cc
#OCSPStapling.cc
SessionAccept.cc
Socks.cc
#SSLCertLookup.cc
#SSLClientUtils.cc
#SSLConfig.cc
#SSLDynlock.cc
#SSLInternal.cc
#SSLNetAccept.cc
#SSLNetProcessor.cc
#SSLNetVConnection.cc
#SSLNextProtocolAccept.cc
#SSLNextProtocolSet.cc
#SSLSessionCache.cc
#SSLUtils.cc
UDPIOEvent.cc
UnixConnection.cc
UnixNetAccept.cc
UnixNet.cc
#UnixNetPages.cc
UnixNetProcessor.cc
UnixNetVConnection.cc
UnixUDPConnection.cc
UnixUDPNet.cc
)


target_link_libraries(tsnet tsutil tsmgmt tseventsystem records pcre dl)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

add_executable(test_i_unixvc.out unixVC_i_test.cc)
target_link_libraries(test_i_unixvc.out tsnet tsutil)
