# Building QtPlayer

This document provides detailed instructions for building QtPlayer on different platforms.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Linux](#linux)
- [macOS](#macos)
- [Windows](#windows)
- [Build Options](#build-options)
- [Troubleshooting](#troubleshooting)

## Prerequisites

### Common Requirements
- CMake 3.10 or higher
- C++11 compatible compiler
- Qt5 (5.12+) or Qt6 (6.0+)
- FFmpeg libraries (4.x or 5.x)
- OpenGL 3.3+ compatible graphics hardware

---

## Linux

### Ubuntu/Debian

#### 1. Install Dependencies

**Option A: Qt5 (Recommended for older systems)**
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    qt5-default \
    qtbase5-dev \
    libqt5opengl5-dev \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libswscale-dev \
    libgl1-mesa-dev \
    git
```

**Option B: Qt6 (For newer systems)**
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    qt6-base-dev \
    qt6-base-dev-tools \
    libgl1-mesa-dev \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libswscale-dev \
    git
```

#### 2. Build

```bash
# Clone repository
git clone https://github.com/yourusername/qtplayer.git
cd qtplayer

# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build (use -j for parallel compilation)
make -j$(nproc)

# Optional: Install system-wide
sudo make install
```

#### 3. Run

```bash
./qtplayer
```

### Fedora/RHEL/CentOS

#### 1. Install Dependencies

```bash
sudo dnf install -y \
    gcc-c++ \
    cmake \
    pkg-config \
    qt5-qtbase-devel \
    qt5-qtbase-gui \
    ffmpeg-devel \
    mesa-libGL-devel \
    git
```

Note: On RHEL/CentOS, you may need to enable EPEL and RPM Fusion repositories for FFmpeg.

#### 2. Build

```bash
git clone https://github.com/yourusername/qtplayer.git
cd qtplayer
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Arch Linux

```bash
# Install dependencies
sudo pacman -S base-devel cmake qt5-base qt5-tools ffmpeg mesa git

# Build
git clone https://github.com/yourusername/qtplayer.git
cd qtplayer
mkdir build
cd build
cmake ..
make -j$(nproc)
```

---

## macOS

### Using Homebrew

#### 1. Install Homebrew
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

#### 2. Install Dependencies

```bash
brew install cmake qt@5 ffmpeg pkg-config
```

Or for Qt6:
```bash
brew install cmake qt@6 ffmpeg pkg-config
```

#### 3. Set Qt Path

```bash
# For Qt5
export Qt5_DIR=$(brew --prefix qt@5)/lib/cmake/Qt5

# For Qt6
export Qt6_DIR=$(brew --prefix qt@6)/lib/cmake/Qt6
```

#### 4. Build

```bash
git clone https://github.com/yourusername/qtplayer.git
cd qtplayer
mkdir build
cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

#### 5. Run

```bash
./qtplayer
```

Or create an app bundle:
```bash
# The app bundle is created automatically
open qtplayer.app
```

---

## Windows

### Using Visual Studio and vcpkg

#### 1. Install Visual Studio
- Download and install Visual Studio 2019 or later
- Include "Desktop development with C++" workload

#### 2. Install CMake
- Download from https://cmake.org/download/
- Add to PATH during installation

#### 3. Install vcpkg (Package Manager)

```cmd
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
```

#### 4. Install Dependencies via vcpkg

```cmd
vcpkg install qt5:x64-windows ffmpeg:x64-windows
vcpkg integrate install
```

#### 5. Build

```cmd
git clone https://github.com/yourusername/qtplayer.git
cd qtplayer
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

#### 6. Run

```cmd
Release\qtplayer.exe
```

### Using MSYS2 (MinGW)

#### 1. Install MSYS2
- Download from https://www.msys2.org/
- Run MSYS2 MinGW 64-bit terminal

#### 2. Install Dependencies

```bash
pacman -S mingw-w64-x86_64-cmake \
          mingw-w64-x86_64-qt5 \
          mingw-w64-x86_64-ffmpeg \
          mingw-w64-x86_64-toolchain \
          git
```

#### 3. Build

```bash
git clone https://github.com/yourusername/qtplayer.git
cd qtplayer
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make -j$(nproc)
```

---

## Build Options

### CMake Options

```bash
# Specify Qt version explicitly
cmake -DQt5_DIR=/path/to/qt5/lib/cmake/Qt5 ..
cmake -DQt6_DIR=/path/to/qt6/lib/cmake/Qt6 ..

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..

# Specify installation prefix
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..

# Verbose build output
cmake --build . --verbose
```

### Build Types
- **Debug**: Includes debug symbols, no optimization
- **Release**: Optimized for performance
- **RelWithDebInfo**: Optimized with debug info
- **MinSizeRel**: Optimized for size

---

## Troubleshooting

### Qt Not Found

**Problem**: CMake cannot find Qt

**Solution**:
```bash
# Set Qt directory manually
cmake -DCMAKE_PREFIX_PATH=/path/to/qt ..

# For Qt5
cmake -DQt5_DIR=/path/to/qt/lib/cmake/Qt5 ..

# For Qt6
cmake -DQt6_DIR=/path/to/qt/lib/cmake/Qt6 ..
```

### FFmpeg Not Found

**Problem**: CMake cannot find FFmpeg libraries

**Solution (Linux)**:
```bash
# Install pkg-config
sudo apt-get install pkg-config

# Install FFmpeg dev packages
sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev

# Check FFmpeg installation
pkg-config --modversion libavcodec
```

**Solution (Windows)**:
- Ensure FFmpeg is installed via vcpkg or MSYS2
- Set `FFMPEG_ROOT` environment variable
- Add FFmpeg bin directory to PATH

### OpenGL Issues

**Problem**: OpenGL headers not found

**Solution (Linux)**:
```bash
sudo apt-get install libgl1-mesa-dev
```

**Solution (macOS)**:
OpenGL is included in macOS SDK (no action needed)

**Solution (Windows)**:
OpenGL is included in graphics drivers (update GPU drivers if needed)

### Linking Errors

**Problem**: Undefined references during linking

**Solution**:
- Ensure all dependencies are installed
- Check that 32-bit/64-bit versions match
- Rebuild with `make clean && make`

### Runtime Issues

**Problem**: Application won't start or crashes

**Check Qt plugins**:
```bash
# Linux
export QT_DEBUG_PLUGINS=1
./qtplayer

# Check for missing libraries
ldd ./qtplayer
```

**Check FFmpeg codecs**:
```bash
# Test FFmpeg installation
ffmpeg -version
ffmpeg -codecs | grep h264
```

### Missing Shared Libraries

**Problem (Linux)**: Error loading shared libraries

**Solution**:
```bash
# Add library path
export LD_LIBRARY_PATH=/path/to/qt/lib:$LD_LIBRARY_PATH

# Or use ldconfig
sudo ldconfig
```

**Problem (macOS)**: Library not loaded

**Solution**:
```bash
# Fix with install_name_tool if needed
otool -L qtplayer  # Check dependencies
```

---

## Performance Tips

### Optimization Flags

For maximum performance:
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native" ..
```

### Static Linking (Linux)

For portable binary:
```bash
cmake -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++" ..
```

---

## Development Build

For active development:

```bash
# Debug build with all warnings
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-Wall -Wextra" ..

# Enable verbose build
cmake --build . -- VERBOSE=1

# Quick rebuild
make -j$(nproc)
```

---

## Cross-Compilation

### Linux to Windows (MinGW)

```bash
# Install cross-compiler
sudo apt-get install mingw-w64

# Configure for Windows target
cmake -DCMAKE_TOOLCHAIN_FILE=mingw-w64-toolchain.cmake ..
make
```

---

## Testing the Build

After building, test with a sample video:

```bash
# Download test video
wget http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4

# Run player
./qtplayer
# Then use File → Open File to load BigBuckBunny.mp4
```

---

## Getting Help

If you encounter issues:

1. Check this troubleshooting guide
2. Verify all dependencies are correctly installed
3. Check CMake output for warnings/errors
4. Review build logs
5. Open an issue on GitHub with:
   - OS and version
   - Qt version
   - FFmpeg version
   - Full error message
   - CMake output

---

## Additional Resources

- [Qt Documentation](https://doc.qt.io/)
- [FFmpeg Documentation](https://ffmpeg.org/documentation.html)
- [CMake Documentation](https://cmake.org/documentation/)
- [OpenGL Documentation](https://www.opengl.org/documentation/)
