PROJECT(test_lock)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lpcre -g -O0")
SET(CMAKE_SOURCE_DIR .)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ ${CMAKE_CURRENT_SOURCE_DIR}) 

FIND_LIBRARY(RUNTIME_LIB /usr/lib  /usr/local/lib ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts ${CMAKE_CURRENT_SOURCE_DIR})

add_library(tseventsystem SHARED 
IOBuffer.cc
Lock.cc
Thread.cc
ProxyAllocator.cc
IOBuffer.cc
)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

add_executable(test_lock.out lock_test.cc)
add_executable(test_thread.out thread_test.cc)
add_executable(test_buffer.out buffer_test.cc)
target_link_libraries(test_lock.out tseventsystem tsutil pcre dl)
target_link_libraries(test_thread.out tseventsystem tsutil pcre dl)
target_link_libraries(test_buffer.out tseventsystem tsutil pcre dl)
