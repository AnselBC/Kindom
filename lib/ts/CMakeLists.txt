cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 -pthread -lpcre -ldl")
INCLUDE_DIRECTORIES(../)
aux_source_directory(. DIR_SRCS)
add_library(tsutil SHARED
ink_mutex.cc
Diags.cc 
BaseLogFile.cc
ink_memory.cc
ink_assert.cc
ink_error.cc 
ink_time.cc
ink_hrtime.cc
ink_cap.cc
ink_file.cc
ink_string.cc
ParseRules.cc
Regex.cc 
SourceLocation.cc
ink_queue.cc
ink_resource.cc
hugepages.cc
)

target_link_libraries(tsutil) 
