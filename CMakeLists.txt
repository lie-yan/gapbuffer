cmake_minimum_required(VERSION 3.10)
project(gapbuffer)

set(CMAKE_CXX_STANDARD 17)

include_directories(~/workplace/third-party/GSL/include)
include_directories(include)

add_executable(gapbuffer
        demo/gap_buffer_demo.cc
        include/gap_buffer.h)
