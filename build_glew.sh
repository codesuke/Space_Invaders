#!/bin/bash
# Build GLEW for MinGW

GLEW_VERSION="2.1.0"
GLEW_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/utils/glew-${GLEW_VERSION}"
BUILD_DIR="${GLEW_DIR}/build_mingw"

echo "Building GLEW ${GLEW_VERSION} for MinGW..."

# Create build directory
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Download GLEW source if not already present
if [ ! -f "glew.c" ]; then
    echo "Downloading GLEW source..."
    cd ..
    if [ -d "src" ]; then
        echo "GLEW source already extracted"
    else
        # Try to use git clone
        git clone --depth 1 https://github.com/nigels-com/glew.git temp_glew
        cp -r temp_glew/src src
        rm -rf temp_glew
    fi
    cd "${BUILD_DIR}"
fi

# Compile GLEW static library
echo "Compiling GLEW..."
gcc -c ../src/glew.c -I../include -DGLEW_STATIC -o glew.o
ar rcs libglew32.a glew.o

# Copy to lib directory
if [ -f "libglew32.a" ]; then
    cp libglew32.a "${GLEW_DIR}/lib/Release/libglew32.a"
    echo "GLEW library built successfully!"
    echo "Library: ${GLEW_DIR}/lib/Release/libglew32.a"
else
    echo "Error: Failed to build GLEW library"
    exit 1
fi
