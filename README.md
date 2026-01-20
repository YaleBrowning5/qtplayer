# QtPlayer - Video Player

A feature-rich video player application built with Qt, OpenGL, and FFmpeg.

![QtPlayer](https://img.shields.io/badge/Qt-5%20%7C%206-green.svg)
![FFmpeg](https://img.shields.io/badge/FFmpeg-4.x%20%7C%205.x-blue.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

## Features

### Core Functionality
- **Multi-format Support**: Play MP4, MKV, AVI, MOV, WMV, FLV, and WebM videos
- **Hardware-Accelerated Rendering**: OpenGL-based video rendering for smooth playback
- **FFmpeg Integration**: Professional-grade video decoding using FFmpeg API

### Playback Controls
- **Play/Pause**: Toggle playback with spacebar or button
- **Progress Slider**: Seek to any position in the video
- **Speed Control**: Adjust playback speed (0.25x, 0.5x, 0.75x, 1.0x)
- **Video Rotation**: Rotate video in 90° increments (0°, 90°, 180°, 270°)
- **Fullscreen Mode**: Immersive viewing experience with auto-hide controls

### User Interface
- **Playlist Sidebar**: Browse and select videos from a directory
- **Dark Theme**: Modern, eye-friendly interface
- **Keyboard Shortcuts**: Quick access to common functions
- **Time Display**: Current position and total duration

### Advanced Features
- **Auto-Aspect Ratio**: Maintains video proportions without distortion
- **Rotation Adaptation**: Automatically adjusts viewport when video is rotated
- **Smart Fullscreen**: Auto-hide controls and playlist with mouse interaction
- **Mouse Idle Detection**: Hide cursor and controls after 3 seconds of inactivity

## Screenshots

### Main Window
![Main Window](docs/screenshots/main_window.png)

### Fullscreen Mode
![Fullscreen Mode](docs/screenshots/fullscreen_mode.png)

## Installation

### Prerequisites

#### Ubuntu/Debian
```bash
# Qt5
sudo apt-get update
sudo apt-get install qt5-default qtbase5-dev libqt5opengl5-dev

# Qt6 (alternative)
sudo apt-get install qt6-base-dev qt6-base-dev-tools libgl1-mesa-dev

# FFmpeg
sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev

# Build tools
sudo apt-get install cmake build-essential pkg-config
```

#### Fedora/RHEL
```bash
# Qt5
sudo dnf install qt5-qtbase-devel qt5-qtbase-gui

# FFmpeg
sudo dnf install ffmpeg-devel

# Build tools
sudo dnf install cmake gcc-c++ pkg-config
```

#### macOS
```bash
# Using Homebrew
brew install qt ffmpeg cmake pkg-config
```

#### Windows
1. Download and install [Qt](https://www.qt.io/download) (Qt5 or Qt6)
2. Download FFmpeg development libraries from [FFmpeg website](https://ffmpeg.org/download.html)
3. Install [CMake](https://cmake.org/download/)
4. Install Visual Studio or MinGW for compilation

For detailed installation instructions, see [BUILD.md](docs/BUILD.md).

## Building

```bash
# Clone the repository
git clone https://github.com/yourusername/qtplayer.git
cd qtplayer

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make

# Run
./qtplayer
```

For platform-specific build instructions, see [BUILD.md](docs/BUILD.md).

## Usage

### Opening Videos

**Single File**:
- File → Open File... (Ctrl+O)
- Select a video file

**Directory**:
- File → Open Directory... (Ctrl+D)
- Select a folder containing videos
- All videos will appear in the playlist

### Playback Controls

**Keyboard Shortcuts**:
- `Space`: Play/Pause
- `F` or `F11`: Toggle Fullscreen
- `Esc`: Exit Fullscreen
- `Left Arrow`: Rewind 5 seconds
- `Right Arrow`: Forward 5 seconds

**Mouse Controls**:
- Click progress bar to seek
- Drag progress slider for precise seeking
- In fullscreen: Move mouse to left edge to show playlist
- In fullscreen: Move mouse to show controls

### Video Rotation
Click the rotate button (↻) to rotate the video 90° clockwise. The video will automatically adapt to fill the window while maintaining aspect ratio.

### Speed Control
Use the speed dropdown to adjust playback speed:
- 0.25x - Quarter speed
- 0.5x - Half speed
- 0.75x - Three-quarter speed
- 1.0x - Normal speed

## Architecture

### Components

- **FFmpegDecoder**: Video decoding using FFmpeg API
- **VideoRenderer**: OpenGL-based rendering with shader support
- **VideoPlayer**: Core playback logic and timing control
- **PlaylistWidget**: File list management
- **ControlBar**: Playback controls UI
- **MainWindow**: Main application window and event handling

### Threading Model
- **UI Thread**: Handles user interface and OpenGL rendering
- **Decoder**: FFmpeg decoding operations (integrated with main loop)
- **Timer-based Playback**: Frame delivery controlled by QTimer

### Technology Stack
- **Qt5/Qt6**: Cross-platform GUI framework
- **OpenGL 3.3+**: Hardware-accelerated rendering
- **FFmpeg 4.x/5.x**: Video decoding and format support
- **CMake**: Build system

## Configuration

### Supported Video Formats
- MP4 (H.264, H.265)
- MKV (Matroska)
- AVI
- MOV (QuickTime)
- WMV (Windows Media)
- FLV (Flash Video)
- WebM

### System Requirements
- **OS**: Linux, macOS, Windows
- **RAM**: 2GB minimum, 4GB recommended
- **GPU**: OpenGL 3.3 compatible graphics card
- **CPU**: Dual-core processor or better

## Troubleshooting

### Video won't play
- Ensure FFmpeg libraries are properly installed
- Check if the video codec is supported
- Try a different video file to verify installation

### Black screen during playback
- Update graphics drivers
- Verify OpenGL 3.3+ support: `glxinfo | grep "OpenGL version"` (Linux)

### Build errors
- Verify all dependencies are installed
- Check CMake output for missing packages
- See [BUILD.md](docs/BUILD.md) for detailed troubleshooting

## Known Issues

- Some exotic video codecs may not be supported
- Very high resolution videos (8K+) may require powerful hardware
- Seeking in some video formats may be less precise

## Future Enhancements

- [ ] Audio playback support
- [ ] Subtitle support (SRT, ASS)
- [ ] Hardware-accelerated decoding (VAAPI, NVDEC)
- [ ] Playlist save/load functionality
- [ ] Volume control
- [ ] Frame-by-frame stepping
- [ ] Video filters and effects
- [ ] Screen capture/recording
- [ ] Network streaming support

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Qt Framework for the excellent GUI toolkit
- FFmpeg team for the powerful multimedia libraries
- OpenGL for hardware-accelerated graphics

## Contact

Project Link: [https://github.com/yourusername/qtplayer](https://github.com/yourusername/qtplayer)

## Authors

- Initial implementation - Video player with OpenGL rendering and FFmpeg integration

---

**Note**: This is an educational project demonstrating integration of Qt, OpenGL, and FFmpeg for video playback.