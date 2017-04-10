cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 -pthread -lpcre -ldl -Wno-invalid-offsetof -lcrypto")
INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../../iocore/eventsystem
${CMAKE_CURRENT_SOURCE_DIR}/../mgmt/api/include
${CMAKE_CURRENT_SOURCE_DIR}/../mgmt/utils
${CMAKE_CURRENT_SOURCE_DIR}/../mgmt
${CMAKE_CURRENT_SOURCE_DIR}/../
${CMAKE_CURRENT_SOURCE_DIR}
/usr/include/tcl8.5/
)


aux_source_directory(. DIR_SRCS)
add_library(tsutil STATIC 
Arena.cc
BaseLogFile.cc
Bitops.cc
CompileParseRules.cc
ConsistentHash.cc
ContFlags.cc
Diags.cc
EventNotify.cc
Hash.cc
HashFNV.cc
HashMD5.cc
HashSip.cc
HostLookup.cc
hugepages.cc
ink_args.cc
ink_assert.cc
ink_base64.cc
ink_cap.cc
ink_code.cc
ink_defs.cc
InkErrno.cc
ink_error.cc
ink_file.cc
ink_hash_table.cc
ink_hrtime.cc
ink_inet.cc
ink_memory.cc
ink_mutex.cc
ink_queue.cc
ink_queue_utils.cc
ink_rand.cc
#ink_res_init.cc
#ink_res_mkquery.cc
ink_resource.cc
ink_rwlock.cc
ink_sock.cc
ink_sprintf.cc
ink_stack_trace.cc
ink_string.cc
ink_string++.cc
ink_sys_control.cc
ink_syslog.cc
ink_thread.cc
ink_time.cc
ink_uuid.cc
IpMap.cc
IpMapConf.cc
#IpMapTest.cc
Layout.cc
llqueue.cc
#load_http_hdr.cc
lockfile.cc
MatcherUtils.cc
MemView.cc
MMH.cc
ParseRules.cc
RawHashTable.cc
RbTree.cc
Regex.cc
Regression.cc
signals.cc
SourceLocation.cc
#TestHttpHeader.cc
TextBuffer.cc
Tokenizer.cc
Vec.cc
Version.cc
X509HostnameValidator.cc
)

target_link_libraries(tsutil crypto) 
