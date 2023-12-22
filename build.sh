#!/bin/sh

sudo apt-get install libssl-dev
sudo apt install cmake

sudo find / -name "libssl*" -or -name "libcrypto*"
export CMAKE_PREFIX_PATH=/path/to/libssl.so

mkdir build
cd build
cmake ..
make
