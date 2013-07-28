cmake_minimum_required(VERSION 2.8)

PROJECT(md5check)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_C_FLAGS "-g -Wall")

include_directories(/usr/local/include)

set(src_common md5.c file.cpp usage.cpp) 
set(src_md5create main_md5create.cpp ${src_common})
set(src_md5check main_md5check.cpp ${src_common})

add_executable(md5create ${src_md5create})
add_executable(md5check ${src_md5check})

INSTALL(TARGETS md5check md5create RUNTIME DESTINATION bin)
