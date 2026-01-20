#!/bin/bash
# Build verification script for QtPlayer

set -e

echo "================================================"
echo "QtPlayer Build Verification Script"
echo "================================================"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓${NC} $2"
    else
        echo -e "${RED}✗${NC} $2"
    fi
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

# Check for required tools
echo "Checking required tools..."
echo "-----------------------------------"

command -v cmake >/dev/null 2>&1
print_status $? "CMake found"

command -v pkg-config >/dev/null 2>&1
print_status $? "pkg-config found"

command -v g++ >/dev/null 2>&1 || command -v clang++ >/dev/null 2>&1
print_status $? "C++ compiler found"

echo ""

# Check for Qt
echo "Checking Qt installation..."
echo "-----------------------------------"

QT_VERSION=""
if pkg-config --exists Qt5Core 2>/dev/null; then
    QT_VERSION=$(pkg-config --modversion Qt5Core)
    print_status 0 "Qt5 found (version $QT_VERSION)"
elif pkg-config --exists Qt6Core 2>/dev/null; then
    QT_VERSION=$(pkg-config --modversion Qt6Core)
    print_status 0 "Qt6 found (version $QT_VERSION)"
else
    print_status 1 "Qt not found"
    print_warning "Install Qt5 or Qt6 to build QtPlayer"
fi

echo ""

# Check for FFmpeg
echo "Checking FFmpeg libraries..."
echo "-----------------------------------"

if pkg-config --exists libavcodec 2>/dev/null; then
    AVCODEC_VERSION=$(pkg-config --modversion libavcodec)
    print_status 0 "libavcodec found (version $AVCODEC_VERSION)"
else
    print_status 1 "libavcodec not found"
fi

if pkg-config --exists libavformat 2>/dev/null; then
    AVFORMAT_VERSION=$(pkg-config --modversion libavformat)
    print_status 0 "libavformat found (version $AVFORMAT_VERSION)"
else
    print_status 1 "libavformat not found"
fi

if pkg-config --exists libavutil 2>/dev/null; then
    AVUTIL_VERSION=$(pkg-config --modversion libavutil)
    print_status 0 "libavutil found (version $AVUTIL_VERSION)"
else
    print_status 1 "libavutil not found"
fi

if pkg-config --exists libswscale 2>/dev/null; then
    SWSCALE_VERSION=$(pkg-config --modversion libswscale)
    print_status 0 "libswscale found (version $SWSCALE_VERSION)"
else
    print_status 1 "libswscale not found"
fi

echo ""

# Check for OpenGL
echo "Checking OpenGL..."
echo "-----------------------------------"

if [ -f /usr/include/GL/gl.h ] || [ -f /usr/local/include/GL/gl.h ]; then
    print_status 0 "OpenGL headers found"
elif [ "$(uname)" == "Darwin" ]; then
    print_status 0 "OpenGL (macOS framework)"
else
    print_status 1 "OpenGL headers not found"
fi

echo ""

# Source file verification
echo "Verifying source files..."
echo "-----------------------------------"

required_files=(
    "CMakeLists.txt"
    "src/main.cpp"
    "src/MainWindow.h"
    "src/MainWindow.cpp"
    "src/VideoPlayer.h"
    "src/VideoPlayer.cpp"
    "src/VideoRenderer.h"
    "src/VideoRenderer.cpp"
    "src/FFmpegDecoder.h"
    "src/FFmpegDecoder.cpp"
    "src/PlaylistWidget.h"
    "src/PlaylistWidget.cpp"
    "src/ControlBar.h"
    "src/ControlBar.cpp"
)

all_files_present=true
for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        print_status 0 "$file"
    else
        print_status 1 "$file (missing)"
        all_files_present=false
    fi
done

echo ""

# Documentation verification
echo "Verifying documentation..."
echo "-----------------------------------"

doc_files=("README.md" "docs/BUILD.md" "LICENSE" "CONTRIBUTING.md" "CHANGELOG.md")

for file in "${doc_files[@]}"; do
    if [ -f "$file" ]; then
        print_status 0 "$file"
    else
        print_status 1 "$file (missing)"
    fi
done

echo ""

# Try to configure with CMake
echo "Attempting CMake configuration..."
echo "-----------------------------------"

if [ "$all_files_present" = true ]; then
    if [ -d "build" ]; then
        print_warning "Build directory exists, cleaning..."
        rm -rf build
    fi
    
    mkdir -p build
    cd build
    
    if cmake .. >/dev/null 2>&1; then
        print_status 0 "CMake configuration successful"
        
        # Try to build
        echo ""
        echo "Attempting build..."
        echo "-----------------------------------"
        
        if make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1) >/dev/null 2>&1; then
            print_status 0 "Build successful"
            echo ""
            echo -e "${GREEN}✓ QtPlayer built successfully!${NC}"
            echo "Run with: ./build/qtplayer"
        else
            print_status 1 "Build failed"
            print_warning "Check build output for errors"
        fi
    else
        print_status 1 "CMake configuration failed"
        print_warning "Install missing dependencies (Qt, FFmpeg)"
    fi
    
    cd ..
else
    print_warning "Skipping build due to missing source files"
fi

echo ""
echo "================================================"
echo "Verification complete"
echo "================================================"
echo ""
echo "For detailed build instructions, see docs/BUILD.md"
echo ""
