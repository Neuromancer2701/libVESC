cmake_minimum_required(VERSION 3.12)
project(libVESC)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(inc
                    inc/g3log
                    inc/libserial)

add_executable(libVESC
        inc/datatypes.h
        commands.cpp
        commands.h
        packet.cpp
        packet.h
        )

TARGET_LINK_LIBRARIES(libVESC
        libserial_static
        )