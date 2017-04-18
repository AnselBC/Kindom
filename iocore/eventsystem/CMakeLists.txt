PROJECT(test_lock)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lpcre -g -O0 -Wno-invalid-offsetof ")
SET(CMAKE_SOURCE_DIR .)


INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ 
${CMAKE_CURRENT_SOURCE_DIR}
) 

FIND_LIBRARY(RUNTIME_LIB 
/usr/lib  
/usr/local/lib 
${CMAKE_CURRENT_SOURCE_DIR}/../../lib
)

link_directories(
${CMAKE_CURRENT_SOURCE_DIR}/../../lib 
${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(keventsystem STATIC 
EThread.cc
EventProcessor.cc
PQ-List.cc
Processor.cc
ProtectedQueue.cc
Thread.cc
)


target_link_libraries(keventsystem tsutil)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

add_executable(test_thread.out thread_test.cc)
add_executable(test_action.out action_test.cc)
add_executable(test_processor.out processor_test.cc) 
add_executable(test_iobuffer.out iobuffer_test.cc) 
target_link_libraries(test_thread.out keventsystem)
target_link_libraries(test_action.out keventsystem)
target_link_libraries(test_processor.out keventsystem)
target_link_libraries(test_iobuffer.out keventsystem)
