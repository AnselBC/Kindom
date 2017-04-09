PROJECT(test_lock)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lpcre -g -O0 -Wno-invalid-offsetof ")
SET(CMAKE_SOURCE_DIR .)


INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/
${CMAKE_CURRENT_SOURCE_DIR}
) 

FIND_LIBRARY(
RUNTIME_LIB 
/usr/lib  
/usr/local/lib 
${CMAKE_CURRENT_SOURCE_DIR}/../eventsystem/
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts
)

link_directories(
${CMAKE_CURRENT_SOURCE_DIR}/../eventsystem/
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts 
${CMAKE_CURRENT_SOURCE_DIR}
)

#add_library(tsnet STATIC)


#target_link_libraries(tseventsystem tsutil pcre dl)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

add_executable(test_i_netvc.out netProcessor_i_test.cc)
target_link_libraries(test_i_netvc.out tseventsystem)
