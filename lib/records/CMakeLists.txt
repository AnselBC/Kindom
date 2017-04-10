cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 -pthread -lpcre -ldl -Wno-invalid-offsetof")
INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../../iocore/eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../mgmt/api/include
${CMAKE_CURRENT_SOURCE_DIR}/../mgmt/utils
${CMAKE_CURRENT_SOURCE_DIR}/../mgmt
${CMAKE_CURRENT_SOURCE_DIR}/../
${CMAKE_CURRENT_SOURCE_DIR}
/usr/include/tcl8.5/
)

FIND_LIBRARY(RUNTIME_LIB 
/usr/lib  
/usr/local/lib 
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts/
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/records/
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/mgmt/
)


link_directories(
${CMAKE_CURRENT_SOURCE_DIR}/../../iocore/eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/records
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/mgmt/
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ts 
${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ 
${CMAKE_CURRENT_SOURCE_DIR}
)

aux_source_directory(. DIR_SRCS)
add_library(records STATIC 
P_RecCore.cc
RecConfigParse.cc
RecCore.cc
RecDebug.cc
RecFile.cc
#RecHttp.cc
RecLocal.cc
RecMessage.cc
RecMutex.cc
RecProcess.cc
RecRawStats.cc
RecUtils.cc
)

target_link_libraries(records tsutil tsmgmt tseventsystem pcre dl tcl8.5) 

