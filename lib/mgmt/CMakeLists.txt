cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 -pthread -lpcre -ldl -Wno-invalid-offsetof ")
INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../../iocore/eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../records
${CMAKE_CURRENT_SOURCE_DIR}/utils
${CMAKE_CURRENT_SOURCE_DIR}/../
${CMAKE_CURRENT_SOURCE_DIR}
/usr/include/tcl8.5/
)

aux_source_directory(. DIR_SRCS)
add_library(tsmgmt SHARED 
Alarms.cc
BaseManager.cc
FileManager.cc
#LocalManager.cc
MultiFile.cc
#ProcessManager.cc
ProxyConfig.cc
RecordsConfig.cc
RecordsConfigUtils.cc
Rollback.cc
WebMgmtUtils.cc
utils/ExpandingArray.cc
utils/MgmtLocalCleanup.cc
utils/MgmtMarshall.cc
utils/MgmtProcessCleanup.cc
utils/MgmtSocket.cc
utils/MgmtUtils.cc
)

#target_link_libraries(tsutil) 
