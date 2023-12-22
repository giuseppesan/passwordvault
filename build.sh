#!/bin/sh

sudo apt-get install libssl-dev
sudo apt install libgtest-dev
sudo apt install cmake

mkdir build
cd build
cmake ..
make
