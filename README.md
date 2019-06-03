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

To suport C++17 filesystem  
apt install g++-8 

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 10  
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 20  

sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 10  
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 20 

sudo update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30  
sudo update-alternatives --set cc /usr/bin/gcc  

sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30  
sudo update-alternatives --set c++ /usr/bin/g++   





