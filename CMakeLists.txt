cmake_minimum_required(VERSION 2.8)

PROJECT(md5check)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_C_FLAGS "-g -Wall")

include_directories(/usr/local/include)

set(src_md5check main_md5check.cpp md5.c)
add_executable(md5check ${src_md5check})

INSTALL(TARGETS md5check RUNTIME DESTINATION bin)
