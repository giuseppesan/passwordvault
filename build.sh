#!/bin/bash

# Function to check if a package is installed
check_package() {
    dpkg -s $1 &> /dev/null
    return $?
}
os_name=$(uname -s)

# Check if the OS is Linux
# List of packages to be checked, last 3 are for ubuntu
if [ "$os_name" == "Linux" ]; then
    echo "Linux detected"
    
    # Use the default C++ compiler
    export CXX=$(which clang++)

    if command -v lsb_release > /dev/null 2>&1; then
    # Get the distribution name
        distro_name=$(lsb_release -si)

        # Check if the distribution is Ubuntu
        if [ "$distro_name" == "Ubuntu" ]; then
            echo "Ubuntu detected"
            packages=("libssl-dev" "libgtest-dev" "cmake" "clang" "libx11-dev" "xorg-dev" "libglu1-mesa-dev")
        else
            packages=("libssl-dev" "libgtest-dev" "cmake" "clang")
        fi
    fi
fi

# Check if the OS is macOS
if [ "$os_name" == "Darwin" ]; then
    echo "macOS detected"
    #brew install 
    pagages_mac=("openssl" "googletest" "cmake" "libx11" "xorg-server" "mesa" "gcc")
fi

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

cd "$build_dir"
cmake .. && make -j4
