cmake_minimum_required (VERSION 2.6)
 
project (test_lock)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lpcre -g -O0 -Wno-invalid-offsetof ")

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../lib/ 
${CMAKE_CURRENT_SOURCE_DIR}
) 
 
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/eventsystem)
