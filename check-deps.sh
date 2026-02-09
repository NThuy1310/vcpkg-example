#!/bin/bash

# Script to check and install prerequisites for the C++ project

echo "=== Checking Prerequisites ==="
echo ""

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check CMake
if command_exists cmake; then
    CMAKE_VERSION=$(cmake --version | head -n1)
    echo "✓ CMake found: $CMAKE_VERSION"
else
    echo "✗ CMake not found"
    echo "  Install with: sudo apt install cmake"
    MISSING_DEPS=1
fi

# Check Ninja
if command_exists ninja; then
    NINJA_VERSION=$(ninja --version)
    echo "✓ Ninja found: version $NINJA_VERSION"
else
    echo "✗ Ninja not found"
    echo "  Install with: sudo apt install ninja-build"
    MISSING_DEPS=1
fi

# Check C++ Compiler
if command_exists g++; then
    GCC_VERSION=$(g++ --version | head -n1)
    echo "✓ G++ compiler found: $GCC_VERSION"
elif command_exists clang++; then
    CLANG_VERSION=$(clang++ --version | head -n1)
    echo "✓ Clang++ compiler found: $CLANG_VERSION"
else
    echo "✗ C++ compiler not found"
    echo "  Install with: sudo apt install build-essential"
    MISSING_DEPS=1
fi

# Check vcpkg
echo ""
if [ -n "$VCPKG_ROOT" ]; then
    if [ -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]; then
        echo "✓ vcpkg found at: $VCPKG_ROOT"
    else
        echo "⚠ VCPKG_ROOT is set but toolchain file not found"
        echo "  Path: $VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
    fi
else
    echo "⚠ VCPKG_ROOT not set (vcpkg is optional)"
    echo "  To use vcpkg, set VCPKG_ROOT in your environment:"
    echo "  export VCPKG_ROOT=/path/to/vcpkg"
fi

echo ""
if [ -n "$MISSING_DEPS" ]; then
    echo "=== Missing Dependencies ==="
    echo "Install all required tools with:"
    echo "  sudo apt update"
    echo "  sudo apt install build-essential cmake ninja-build git"
    exit 1
else
    echo "=== All required dependencies are installed! ==="
    echo "You can now build the project with:"
    echo "  cmake --preset linux-debug"
    echo "  cmake --build --preset linux-debug"
fi
