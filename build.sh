#!/bin/sh

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
