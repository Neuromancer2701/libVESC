# libVESC
Port of the VESC tool to C++17 library so it can be used in other programs
The plan is to use this library to communicate to multiple VESC units via USB in an standalone ROS node.

Maintained the license

install these libs for support  
https://github.com/crayzeewulf/libserial.git  
./compile.sh  
make install

https://github.com/KjellKod/g3log.git  
mkdir build && cd build  
cmake ..  
make install -j 8


