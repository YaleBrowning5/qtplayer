# QtPlayer Troubleshooting Guide

This guide helps you diagnose and fix common issues with QtPlayer.

## Table of Contents
- [Build Issues](#build-issues)
- [Runtime Issues](#runtime-issues)
- [Video Playback Issues](#video-playback-issues)
- [UI Issues](#ui-issues)
- [Performance Issues](#performance-issues)

---

## Build Issues

### CMake Cannot Find Qt

**Symptom:**
```
CMake Error: Could not find a package configuration file provided by "Qt5"
```

**Solutions:**

1. **Set Qt path manually:**
   ```bash
   cmake -DCMAKE_PREFIX_PATH=/path/to/qt5 ..
   # or
   cmake -DQt5_DIR=/path/to/qt5/lib/cmake/Qt5 ..
   ```

2. **Install Qt properly:**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install qt5-default qtbase5-dev
   
   # macOS
   brew install qt@5
   export Qt5_DIR=$(brew --prefix qt@5)/lib/cmake/Qt5
   ```

3. **Verify Qt installation:**
   ```bash
   qmake --version  # Should show Qt version
   ```

### CMake Cannot Find FFmpeg

**Symptom:**
```
Package 'libavcodec' not found
```

**Solutions:**

1. **Install FFmpeg development libraries:**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev
   
   # Fedora
   sudo dnf install ffmpeg-devel
   
   # macOS
   brew install ffmpeg
   ```

2. **Verify FFmpeg installation:**
   ```bash
   pkg-config --modversion libavcodec
   ffmpeg -version
   ```

3. **Set PKG_CONFIG_PATH if needed:**
   ```bash
   export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
   ```

### Compilation Errors

**Symptom:**
```
error: 'XXX' was not declared in this scope
```

**Solutions:**

1. **Check C++ standard:**
   ```bash
   # Ensure C++11 or higher
   cmake -DCMAKE_CXX_STANDARD=11 ..
   ```

2. **Update compiler:**
   ```bash
   g++ --version  # Should be 4.8+ for C++11
   ```

3. **Clean build directory:**
   ```bash
   rm -rf build
   mkdir build
   cd build
   cmake ..
   ```

### Linking Errors

**Symptom:**
```
undefined reference to `avcodec_find_decoder'
```

**Solutions:**

1. **Verify FFmpeg libraries are linked:**
   ```bash
   ldd ./qtplayer | grep av
   ```

2. **Check library paths:**
   ```bash
   export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
   ```

3. **Reinstall FFmpeg development packages**

---

## Runtime Issues

### Application Won't Start

**Symptom:** Application crashes immediately or shows no window

**Solutions:**

1. **Check dependencies:**
   ```bash
   # Linux
   ldd ./qtplayer
   
   # macOS
   otool -L ./qtplayer
   ```

2. **Run with debug output:**
   ```bash
   export QT_DEBUG_PLUGINS=1
   ./qtplayer
   ```

3. **Check OpenGL support:**
   ```bash
   # Linux
   glxinfo | grep "OpenGL version"
   
   # Should show 3.3 or higher
   ```

4. **Update graphics drivers**

### Missing Qt Platform Plugin

**Symptom:**
```
This application failed to start because no Qt platform plugin could be initialized
```

**Solutions:**

1. **Set Qt plugin path:**
   ```bash
   export QT_QPA_PLATFORM_PLUGIN_PATH=/path/to/qt/plugins
   ```

2. **Install Qt plugins:**
   ```bash
   sudo apt-get install qt5-qmake qtbase5-dev-tools
   ```

3. **Check available platforms:**
   ```bash
   export QT_DEBUG_PLUGINS=1
   ./qtplayer
   ```

### Segmentation Fault

**Symptom:** Application crashes with "Segmentation fault"

**Solutions:**

1. **Run with debugger:**
   ```bash
   gdb ./qtplayer
   (gdb) run
   # When it crashes:
   (gdb) backtrace
   ```

2. **Check for missing libraries:**
   ```bash
   ldd ./qtplayer | grep "not found"
   ```

3. **Test with valgrind:**
   ```bash
   valgrind --leak-check=full ./qtplayer
   ```

---

## Video Playback Issues

### Video File Won't Open

**Symptom:** Error message "Failed to load video file"

**Solutions:**

1. **Check file format:**
   ```bash
   ffprobe video.mp4
   ```

2. **Verify FFmpeg can decode:**
   ```bash
   ffmpeg -i video.mp4 -f null -
   ```

3. **Check file permissions:**
   ```bash
   ls -l video.mp4
   ```

4. **Try re-encoding video:**
   ```bash
   ffmpeg -i input.mp4 -c:v libx264 -c:a aac output.mp4
   ```

### Black Screen During Playback

**Symptom:** Video plays but screen is black

**Solutions:**

1. **Check OpenGL renderer:**
   ```bash
   glxinfo | grep "OpenGL renderer"
   ```

2. **Update graphics drivers:**
   ```bash
   # NVIDIA
   sudo ubuntu-drivers install
   
   # AMD
   sudo apt-get install mesa-vulkan-drivers
   ```

3. **Try different video format**

4. **Check video codec:**
   ```bash
   ffprobe -show_streams video.mp4 | grep codec_name
   ```

### No Audio

**Symptom:** Video plays but no sound

**Note:** Audio playback is not yet implemented in QtPlayer v1.0. This is a planned feature for future releases.

### Choppy Playback

**Symptom:** Video stutters or drops frames

**Solutions:**

1. **Check CPU usage:**
   ```bash
   top  # Look for qtplayer process
   ```

2. **Reduce video resolution**

3. **Close other applications**

4. **Check video bitrate:**
   ```bash
   ffprobe video.mp4 | grep bitrate
   ```

5. **Enable hardware acceleration (if available)**

### Seeking is Slow or Inaccurate

**Symptom:** Jumping to different position takes time or goes to wrong spot

**Solutions:**

1. **This is normal for some container formats (e.g., FLV)**

2. **Re-encode with better keyframe spacing:**
   ```bash
   ffmpeg -i input.mp4 -g 30 -c:v libx264 output.mp4
   ```

3. **Use MP4 or MKV containers for better seeking**

---

## UI Issues

### Controls Not Responding

**Symptom:** Clicking buttons doesn't work

**Solutions:**

1. **Click on video area to focus window**

2. **Check if video is loaded:**
   - Try File → Open File
   - Verify video appears in title bar

3. **Restart application**

### Fullscreen Mode Problems

**Symptom:** Can't enter/exit fullscreen

**Solutions:**

1. **Use keyboard shortcut:** Press `F` or `Esc`

2. **Check window manager (Linux):**
   ```bash
   # Some window managers may interfere
   # Try different desktop environment
   ```

3. **Restart in windowed mode**

### Playlist Not Showing Videos

**Symptom:** Playlist is empty after opening directory

**Solutions:**

1. **Check directory contains video files:**
   ```bash
   ls *.mp4 *.mkv *.avi
   ```

2. **Verify file extensions are supported**

3. **Check file permissions**

4. **Try absolute path instead of relative**

### Rotation Button Not Working

**Symptom:** Video doesn't rotate when button is clicked

**Solutions:**

1. **Wait for video to load completely**

2. **Check OpenGL support**

3. **Try different video file**

---

## Performance Issues

### High CPU Usage

**Symptom:** CPU usage is very high during playback

**Solutions:**

1. **This is normal for software decoding**

2. **Reduce playback speed**

3. **Use hardware-accelerated video format (H.264)**

4. **Lower video resolution**

### High Memory Usage

**Symptom:** Application uses too much RAM

**Solutions:**

1. **Close other applications**

2. **This is normal for high-resolution videos**

3. **Check for memory leaks:**
   ```bash
   valgrind --leak-check=full ./qtplayer
   ```

### GPU Usage Issues

**Symptom:** GPU usage is low, performance is poor

**Solutions:**

1. **Verify OpenGL is being used:**
   ```bash
   # Linux
   __GL_SYNC_TO_VBLANK=0 ./qtplayer
   ```

2. **Update graphics drivers**

3. **Check GPU acceleration:**
   ```bash
   glxinfo | grep "direct rendering"
   # Should show "Yes"
   ```

---

## Platform-Specific Issues

### Linux

**Wayland vs X11:**
- Try forcing X11: `export QT_QPA_PLATFORM=xcb`
- Or forcing Wayland: `export QT_QPA_PLATFORM=wayland`

**Missing libraries:**
```bash
sudo ldconfig
```

### macOS

**Gatekeeper issues:**
```bash
xattr -cr qtplayer.app
```

**Qt not in PATH:**
```bash
export PATH="/usr/local/opt/qt/bin:$PATH"
```

### Windows

**DLL not found:**
- Copy Qt DLLs to executable directory
- Or add Qt bin directory to PATH

**Visual C++ Runtime missing:**
- Install Visual C++ Redistributable

---

## Debug Mode

Build in debug mode for more information:

```bash
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
gdb ./qtplayer
```

## Logging

Enable verbose Qt logging:

```bash
export QT_LOGGING_RULES="*.debug=true"
./qtplayer 2>&1 | tee qtplayer.log
```

---

## Getting More Help

If your issue isn't covered here:

1. **Check existing GitHub issues**
2. **Search Qt documentation**: https://doc.qt.io/
3. **Search FFmpeg documentation**: https://ffmpeg.org/documentation.html
4. **Open a new GitHub issue** with:
   - Operating system and version
   - Qt version (`qmake --version`)
   - FFmpeg version (`ffmpeg -version`)
   - Complete error message
   - Steps to reproduce
   - Build configuration
   - Sample video file (if applicable)

---

## Useful Diagnostic Commands

```bash
# System info
uname -a

# Qt version
qmake --version

# FFmpeg version and configuration
ffmpeg -version
ffmpeg -codecs | grep h264

# OpenGL version (Linux)
glxinfo | grep "OpenGL version"

# Check library dependencies
ldd ./qtplayer  # Linux
otool -L ./qtplayer  # macOS

# CMake cache
cat build/CMakeCache.txt | grep -i qt
cat build/CMakeCache.txt | grep -i ffmpeg

# Qt plugins
ls $QTDIR/plugins/platforms/
```

---

**Need more help?** Open an issue on GitHub: https://github.com/yourusername/qtplayer/issues
