#!/bin/bash

# Function to check if a package is installed
check_package() {
    dpkg -s $1 &> /dev/null
    return $?
}

# List of packages to be checked
packages=("libssl-dev" "libgtest-dev" "cmake" "libx11-dev" "xorg-dev" "libglu1-mesa-dev" "clang")
build_dir="build"

echo "checking packages..."

if [ "$1" = "install" ]; then
    # Loop through the packages and check if they are installed
    for package in "${packages[@]}"; do
        if check_package $package; then
            echo "$package is already installed."
        else
            echo "Installing $package..."
            sudo apt-get install $package
            if [ $? -eq 0 ]; then
                echo "$package installed successfully."
            else
                echo "Failed to install $package. Exiting."
                exit 1
            fi
        fi
    done
    echo "All required packages are installed."
fi

if [ "$1" = "install" ] || [ "$1" = "clean" ]; then
    # Delete the directory if it exists
    if [ -d "$build_dir" ]; then
        echo "Directory $build_dir exists - deleting ..."
        rm -r "$build_dir"
    fi

    mkdir "$build_dir"
fi

# Use the default C++ compiler
export CXX=$(which clang++)

cd "$build_dir"
cmake .. && make
