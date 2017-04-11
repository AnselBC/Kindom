cmake_minimum_required (VERSION 2.6)
 
project (KINDOM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lpcre -g -O0 -Wno-invalid-offsetof ")

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../lib/records
${CMAKE_CURRENT_SOURCE_DIR}/eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../lib/ 
${CMAKE_CURRENT_SOURCE_DIR}/raft
${CMAKE_CURRENT_SOURCE_DIR}/net
${CMAKE_CURRENT_SOURCE_DIR}
) 
 
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/eventsystem)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/raft)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/net)

add_executable(kindom main.cc)
target_link_libraries(kindom tsnet tseventsystem tsmgmt records tsutil raft)
 
