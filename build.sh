#!/bin/sh

sudo apt-get install libssl-dev
sudo apt install libgtest-dev
sudo apt install cmake
sudo apt install libx11-dev xorg-dev libglu1-mesa-dev
sudo apt install clang

export CXX=/usr/bin/clang++

build_dir="build"

if [ -d "$build_dir" ]; then
    echo "Directory $build_dir already exists - deleting ..."
    rm -r "$build_dir"
fi

mkdir "$build_dir"
cd "$build_dir"

cmake ..
make
