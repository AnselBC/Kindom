PROJECT(test_lock)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lpcre -g -O0 -Wno-invalid-offsetof ")
SET(CMAKE_SOURCE_DIR .)


INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/records
${CMAKE_CURRENT_SOURCE_DIR}/../eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts 
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ 
${CMAKE_CURRENT_SOURCE_DIR}/../net
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

add_library(raft STATIC 
ProxyPort.cc
ProxySession.cc
ProxyServerMain.cc
ProxySessionAccept.cc
)


target_link_libraries(raft tsnet tsutil tsmgmt tseventsystem records pcre dl)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

