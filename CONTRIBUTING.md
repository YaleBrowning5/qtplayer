# Contributing to QtPlayer

Thank you for your interest in contributing to QtPlayer! This document provides guidelines and instructions for contributing.

## Table of Contents
- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Pull Request Process](#pull-request-process)

## Code of Conduct

This project adheres to a code of conduct that all contributors are expected to uphold. Please be respectful and constructive in your interactions.

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally
   ```bash
   git clone https://github.com/YOUR_USERNAME/qtplayer.git
   cd qtplayer
   ```
3. **Add upstream remote**
   ```bash
   git remote add upstream https://github.com/YaleBrowning5/qtplayer.git
   ```

## Development Setup

Follow the instructions in [BUILD.md](docs/BUILD.md) to set up your development environment.

### Quick Start

```bash
# Install dependencies (Ubuntu/Debian example)
sudo apt-get install qt5-default qtbase5-dev libqt5opengl5-dev \
                     libavcodec-dev libavformat-dev libavutil-dev libswscale-dev \
                     cmake build-essential pkg-config

# Build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:
- Clear, descriptive title
- Steps to reproduce
- Expected vs actual behavior
- Your environment (OS, Qt version, FFmpeg version)
- Screenshots or videos if applicable

### Suggesting Features

Feature requests are welcome! Please:
- Check if the feature has already been requested
- Clearly describe the feature and its use case
- Consider providing mockups or examples

### Code Contributions

1. **Create a branch** for your work
   ```bash
   git checkout -b feature/my-new-feature
   ```

2. **Make your changes**
   - Write clean, readable code
   - Follow the coding standards below
   - Add comments for complex logic
   - Update documentation if needed

3. **Test your changes**
   - Build and run the application
   - Test all affected functionality
   - Verify on different video formats if applicable

4. **Commit your changes**
   ```bash
   git add .
   git commit -m "Add feature: description of change"
   ```

5. **Push to your fork**
   ```bash
   git push origin feature/my-new-feature
   ```

6. **Create a Pull Request** on GitHub

## Coding Standards

### C++ Style

- **Indentation**: 4 spaces (no tabs)
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `VideoPlayer`)
  - Functions/Methods: `camelCase` (e.g., `loadVideo()`)
  - Variables: `camelCase` (e.g., `videoWidth`)
  - Private members: `camelCase` (e.g., `frameBuffer`)
  - Constants: `UPPER_CASE` (e.g., `MAX_BUFFER_SIZE`)

- **Braces**: Opening brace on same line
  ```cpp
  if (condition) {
      // code
  }
  ```

- **Headers**: Use include guards
  ```cpp
  #ifndef CLASSNAME_H
  #define CLASSNAME_H
  // ...
  #endif
  ```

### Qt Conventions

- Use Qt naming conventions for signals/slots
- Prefer Qt containers (`QVector`, `QList`) for Qt objects
- Use `std::` containers for raw data
- Always use `Q_OBJECT` macro in QObject-derived classes
- Use Qt's signal/slot mechanism for inter-object communication

### Memory Management

- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) when appropriate
- Follow RAII principles
- Properly free FFmpeg and OpenGL resources
- Avoid memory leaks

### Comments

- Add comments for complex algorithms
- Document public API methods
- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Keep comments up-to-date with code changes

### Example

```cpp
// Good example
class VideoDecoder : public QObject {
    Q_OBJECT

public:
    explicit VideoDecoder(QObject *parent = nullptr);
    ~VideoDecoder();
    
    // Opens a video file for decoding
    // Returns true on success, false on failure
    bool openFile(const QString &filePath);

signals:
    void frameDecoded(const QImage &frame);
    void errorOccurred(const QString &error);

private:
    AVFormatContext *formatContext;
    int videoWidth;
    int videoHeight;
    
    // Helper function to initialize decoder
    bool initializeDecoder();
};
```

## Testing

### Manual Testing

Before submitting a PR, test:
1. **Video playback**: Test with MP4, MKV files
2. **Controls**: Play, pause, seek, speed change
3. **Rotation**: All four rotation angles
4. **Fullscreen**: Enter/exit, auto-hide controls
5. **Playlist**: Load directory, switch videos
6. **Keyboard shortcuts**: Space, arrows, F, Esc

### Test Videos

Use standard test videos:
- Big Buck Bunny
- Sintel
- Elephants Dream

Available from: https://test-videos.co.uk/

### Performance Testing

- Test with different resolutions (720p, 1080p, 4K)
- Monitor CPU and GPU usage
- Check for memory leaks with valgrind (Linux)

## Pull Request Process

### Before Submitting

- [ ] Code compiles without warnings
- [ ] All features work as expected
- [ ] No memory leaks or crashes
- [ ] Documentation is updated
- [ ] Commit messages are clear and descriptive

### PR Description Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Tested on Linux
- [ ] Tested on macOS
- [ ] Tested on Windows
- [ ] Tested with Qt5
- [ ] Tested with Qt6

## Screenshots
If applicable, add screenshots

## Checklist
- [ ] Code follows project style guidelines
- [ ] Comments added for complex code
- [ ] Documentation updated
- [ ] No new warnings introduced
```

### Review Process

1. Maintainers will review your PR
2. Address any feedback or requested changes
3. Once approved, your PR will be merged
4. You'll be credited in the CHANGELOG

## Development Tips

### Debugging

**Enable Qt debug output:**
```bash
export QT_DEBUG_PLUGINS=1
export QT_LOGGING_RULES="*.debug=true"
./qtplayer
```

**Use debugger:**
```bash
gdb ./qtplayer
(gdb) run
```

**Check for memory leaks (Linux):**
```bash
valgrind --leak-check=full ./qtplayer
```

### Hot Reload

For faster development, use:
```bash
# Watch and rebuild on changes (Linux)
while inotifywait -r src/; do make; done
```

### IDE Setup

**Qt Creator:**
1. Open CMakeLists.txt as project
2. Configure kit with Qt version
3. Set build directory
4. Build and run

**VS Code:**
1. Install C/C++ extension
2. Install CMake Tools extension
3. Configure CMake kit
4. Use "CMake: Build" and "CMake: Debug"

## Areas for Contribution

Looking for ideas? Consider these:

### High Priority
- [ ] Audio playback support
- [ ] Subtitle rendering
- [ ] Hardware-accelerated decoding

### Medium Priority
- [ ] Playlist save/load
- [ ] Video thumbnails in playlist
- [ ] Recent files menu
- [ ] Keyboard shortcut customization

### Low Priority
- [ ] Video filters (brightness, contrast, etc.)
- [ ] Screenshot capture
- [ ] Frame-by-frame mode
- [ ] Multiple video windows

## Questions?

- Open an issue for questions
- Join discussions on GitHub
- Check existing issues and PRs

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to QtPlayer! 🎬
