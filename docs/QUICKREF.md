# QtPlayer Quick Reference

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `Space` | Play/Pause |
| `F` or `F11` | Toggle Fullscreen |
| `Esc` | Exit Fullscreen |
| `←` Left Arrow | Rewind 5 seconds |
| `→` Right Arrow | Forward 5 seconds |
| `Ctrl+O` | Open File |
| `Ctrl+D` | Open Directory |
| `Ctrl+Q` | Quit Application |

## Mouse Controls

### Normal Mode
- **Click Progress Bar**: Jump to position
- **Drag Progress Slider**: Seek to specific position
- **Click Play/Pause Button**: Toggle playback
- **Click Rotate Button**: Rotate video 90° clockwise
- **Click Fullscreen Button**: Enter fullscreen mode

### Fullscreen Mode
- **Move Mouse**: Show controls
- **Mouse to Left Edge**: Show playlist (semi-transparent)
- **Mouse Idle (3s)**: Hide controls and cursor
- **Click Fullscreen Button**: Exit fullscreen
- **Press Esc**: Exit fullscreen

## Control Bar Elements

```
[▶] [===========|=====] [00:42 / 03:15] [1.0x ▼] [↻] [⛶]
 ①          ②               ③            ④      ⑤   ⑥
```

① **Play/Pause Button** - Toggle playback (shows ▶ when paused, ⏸ when playing)  
② **Progress Slider** - Seek through video, shows current position  
③ **Time Display** - Current time / Total duration  
④ **Speed Control** - Choose playback speed (0.25x, 0.5x, 0.75x, 1.0x)  
⑤ **Rotate Button** - Rotate video 90° clockwise  
⑥ **Fullscreen Button** - Toggle fullscreen mode  

## Playback Speed

| Speed | Description |
|-------|-------------|
| 0.25x | Quarter speed (very slow motion) |
| 0.5x | Half speed (slow motion) |
| 0.75x | Three-quarter speed |
| 1.0x | Normal speed (default) |

## Video Rotation

Click the rotate button (↻) to cycle through rotation angles:
- 0° → 90° → 180° → 270° → 0° (cycles)

Rotation resets to 0° when loading a new video.

## Supported Video Formats

| Format | Extension | Notes |
|--------|-----------|-------|
| MP4 | .mp4 | Most common, widely supported |
| MKV | .mkv | Matroska container |
| AVI | .avi | Legacy format |
| MOV | .mov | QuickTime format |
| WMV | .wmv | Windows Media Video |
| FLV | .flv | Flash Video |
| WebM | .webm | Web-optimized format |

## Common Codecs Supported

- **Video**: H.264, H.265/HEVC, VP8, VP9, MPEG-4, etc.
- **Audio**: AAC, MP3, Opus, Vorbis, etc. (not yet implemented in player UI)

*Note: Actual codec support depends on your FFmpeg installation.*

## Tips & Tricks

### Loading Videos

**Single Video:**
1. File → Open File (or `Ctrl+O`)
2. Select video file
3. Video plays automatically

**Multiple Videos (Playlist):**
1. File → Open Directory (or `Ctrl+D`)
2. Select folder with videos
3. All videos appear in left sidebar
4. Click any video to play

### Best Performance

- Use hardware-accelerated video formats (H.264)
- Close other applications for 4K playback
- Ensure graphics drivers are up to date

### Fullscreen Experience

1. Click fullscreen button or press `F`
2. Move mouse to show controls
3. Move mouse to left edge to see playlist
4. Let mouse sit idle for smooth viewing (auto-hides controls)
5. Press `Esc` or click fullscreen button to exit

### Video Rotation Use Cases

- Correct videos recorded in wrong orientation
- View portrait videos in landscape mode
- Adapt smartphone videos to screen

## Troubleshooting

### Video Won't Play
- ✓ Check if format is supported
- ✓ Try opening with File → Open File
- ✓ Ensure FFmpeg is properly installed
- ✓ Check console for error messages

### Playback is Choppy
- ✓ Reduce video resolution
- ✓ Close other applications
- ✓ Update graphics drivers
- ✓ Check CPU/GPU usage

### Controls Not Responding
- ✓ Click on video area to focus window
- ✓ Check if video is actually loaded
- ✓ Try restarting the application

### Fullscreen Issues
- ✓ Try pressing `Esc` to exit
- ✓ Press `F` to toggle
- ✓ Check window manager compatibility (Linux)

## File Menu

```
File
├─ Open File...      (Ctrl+O)  - Open single video file
├─ Open Directory... (Ctrl+D)  - Load all videos from folder
└─ Exit             (Ctrl+Q)  - Quit application
```

## Help Menu

```
Help
└─ About - Show application information
```

## System Requirements

**Minimum:**
- CPU: Dual-core processor
- RAM: 2 GB
- GPU: OpenGL 3.3 support
- OS: Linux, macOS, or Windows

**Recommended:**
- CPU: Quad-core processor
- RAM: 4 GB
- GPU: Dedicated graphics with OpenGL 3.3+
- OS: Modern Linux, macOS 10.14+, or Windows 10+

## Getting Help

- **Documentation**: See README.md and docs/BUILD.md
- **Issues**: Report bugs on GitHub Issues
- **Contributing**: See CONTRIBUTING.md

## Version Info

Check Help → About for:
- Application version
- Qt version
- Feature list

---

**QtPlayer** - A modern video player built with Qt, OpenGL, and FFmpeg
