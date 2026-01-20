# Changelog

All notable changes to QtPlayer will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2024-01-20

### Added
- Initial release of QtPlayer
- Video playback with FFmpeg decoding
- OpenGL-based hardware-accelerated rendering
- Support for multiple video formats (MP4, MKV, AVI, MOV, WMV, FLV, WebM)
- Playlist widget for browsing video files
- Playback controls:
  - Play/Pause button
  - Progress slider with seek functionality
  - Time display (current/total)
  - Playback speed control (0.25x, 0.5x, 0.75x, 1.0x)
  - Video rotation (0°, 90°, 180°, 270°)
  - Fullscreen mode
- Fullscreen features:
  - Auto-hide controls after 3 seconds of mouse inactivity
  - Semi-transparent playlist on mouse hover at left edge
  - Hidden cursor when idle
- Keyboard shortcuts:
  - Space: Play/Pause
  - F: Toggle fullscreen
  - Esc: Exit fullscreen
  - Left Arrow: Rewind 5 seconds
  - Right Arrow: Forward 5 seconds
- Menu system:
  - File menu (Open File, Open Directory, Exit)
  - Help menu (About)
- Automatic video aspect ratio maintenance
- Rotation-aware viewport adaptation
- Dark theme UI
- Cross-platform support (Linux, macOS, Windows)
- CMake build system
- Comprehensive documentation:
  - README.md with features and usage
  - BUILD.md with platform-specific build instructions
  - CONTRIBUTING.md with contribution guidelines

### Technical Implementation
- FFmpegDecoder class: Handles video file decoding using FFmpeg API
- VideoRenderer class: OpenGL 3.3 rendering with custom shaders
- VideoPlayer class: Core playback logic with timer-based frame delivery
- PlaylistWidget class: File list management with video filtering
- ControlBar class: UI controls for playback management
- MainWindow class: Main application window with fullscreen support
- Proper resource management (no memory leaks)
- Thread-safe frame buffer handling
- Mutex protection for shared data

### Dependencies
- Qt5 (5.12+) or Qt6 (6.0+)
- FFmpeg 4.x or 5.x (libavcodec, libavformat, libavutil, libswscale)
- OpenGL 3.3+
- CMake 3.10+

## [Unreleased]

### Planned Features
- Audio playback support
- Subtitle rendering (SRT, ASS)
- Hardware-accelerated decoding (VAAPI, NVDEC, VideoToolbox)
- Playlist save/load functionality
- Volume control
- Frame-by-frame stepping
- Video filters and effects
- Network streaming support (HTTP, RTSP)
- Video thumbnails in playlist
- Recent files menu
- Custom keyboard shortcut configuration
- Screenshot capture
- Multi-window support
- Video information dialog
- Equalizer controls

### Known Issues
- Some exotic video codecs may not be supported (depends on FFmpeg build)
- Seeking in some container formats may be imprecise
- Very high resolution videos (8K+) may require powerful hardware

---

## Version History

### [1.0.0] - 2024-01-20
Initial public release with core video player functionality.

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for information on how to contribute to QtPlayer.

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.
