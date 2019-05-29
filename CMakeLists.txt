cmake_minimum_required(VERSION 3.12)
project(VESC)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(inc
                    inc/g3log
                    inc/libserial)

link_directories(/usr/local/lib/)

add_library(VESC
        inc/datatypes.h
        packet.cpp
        packet.h
        commands.cpp
        commands.h
        )


# use with furture executable
#TARGET_LINK_LIBRARIES(VESC
#        libserial.so
#        libg3logger.so
#        )