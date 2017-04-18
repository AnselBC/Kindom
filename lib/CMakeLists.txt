cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 -pthread -lpcre -ldl -Wno-invalid-offsetof -lcrypto")
INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../../iocore/eventsystem
${CMAKE_CURRENT_SOURCE_DIR}
/usr/include/tcl8.5/
)
aux_source_directory(. DIR_SRCS)
add_library(tsutil SHARED 
hugepages.cc
ink_rand.cc
katomic.cc
khrtime.cc
kmemory.cc
kmutex.cc
SourceLocation.cc
)

#add_executable(test_atomic.out atomic_test.cc)
#target_link_libraries(test_atomic.out tsutil)
