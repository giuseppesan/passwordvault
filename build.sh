#!/bin/bash

# Function to check if a package is installed
check_package() {
    dpkg -s $1 &> /dev/null
    return $?
}

os_name=$(uname -s)
build_dir="build"
action=$1

if [ "$action" = "install" ]; then
    echo "Checking packages..."

    if [ "$os_name" == "Linux" ]; then
        echo "Linux detected"
        export CXX=$(which clang++)

        if command -v lsb_release > /dev/null 2>&1; then
            distro_name=$(lsb_release -si)

            case "$distro_name" in
                Ubuntu)
                    echo "Ubuntu detected"
                    packages=("libssl-dev" "libgtest-dev" "cmake" "clang" "libx11-dev" "xorg-dev" "libglu1-mesa-dev" "libwxgtk3.0-gtk3-dev")
                    ;;
                *)
                    packages=("libssl-dev" "libgtest-dev" "cmake" "clang")
                    ;;
            esac

            for package in "${packages[@]}"; do
                if check_package $package; then
                    echo "$package is already installed."
                else
                    echo "Installing $package..."
                    sudo apt-get install -y $package
                    if [ $? -eq 0 ]; then
                        echo "$package installed successfully."
                    else
                        echo "Failed to install $package. Exiting."
                        exit 1
                    fi
                fi
            done
        else
            echo "lsb_relase command not found - please install all packages manually"
            exit 1
        fi
    elif [ "$os_name" == "Darwin" ]; then
        echo "macOS detected"

        if command -v brew > /dev/null 2>&1; then
            packages=("openssl@3" "googletest" "cmake" "llvm" "libx11" "xorg-server" "mesa")

            for package in "${packages[@]}"; do
                if brew list -1 | grep -q "^$package\$"; then
                    echo "$package is already installed."
                else
                    echo "Installing $package with Homebrew"
                    brew install $package
                fi
            done
        else
            echo "Error: Homebrew is not installed. Please install Homebrew and try again."
            exit 1
        fi
    else
        echo "Unsupported operating system."
        exit 1
    fi

    echo "All required packages are installed."
fi

if [ "$action" = "install" ] || [ "$action" = "clean" ]; then
    if [ -d "$build_dir" ]; then
        echo "Directory $build_dir exists - deleting..."
        rm -r "$build_dir"
    fi

    mkdir "$build_dir"
fi

cd "$build_dir"
cmake .. && make -j4
