cmake_minimum_required(VERSION 3.10)
project(VESC)

set(CMAKE_CXX_STANDARD 20)
include_directories(inc
                    /usr/local/include/libserial)

link_directories(/usr/local/lib/ ${PROJECT_SOURCE_DIR}/lib)


add_library(VESC
        inc/datatypes.h
        packet.cpp
        packet.h
        commands.cpp
        commands.h
        Vesc.cpp
        Vesc.h
        inc/utils.h)


TARGET_LINK_LIBRARIES(VESC
        libserial.so)


#install (TARGETS VESC DESTINATION ${PROJECT_SOURCE_DIR}/lib)

add_executable(testcode main.cpp)
TARGET_LINK_LIBRARIES(testcode VESC)