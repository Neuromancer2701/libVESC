cmake_minimum_required(VERSION 3.12)
project(libVESC)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(inc)

add_executable(libVESC
        inc/datatypes.h
        commands.cpp
        commands.h
        packet.cpp
        packet.h
        vbytearray.cpp
        vbytearray.h)
