cmake_minimum_required(VERSION 3.10)
project(qtplayer VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages (try Qt6 first, fallback to Qt5)
find_package(Qt6 COMPONENTS Core Gui Widgets OpenGL QUIET)
if(Qt6_FOUND)
    message(STATUS "Found Qt6")
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)
    message(STATUS "Found Qt5")
    set(QT_VERSION_MAJOR 5)
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find FFmpeg libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED
    libavcodec
    libavformat
    libavutil
    libswscale
)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/VideoPlayer.cpp
    src/VideoRenderer.cpp
    src/FFmpegDecoder.cpp
    src/PlaylistWidget.cpp
    src/ControlBar.cpp
)

# Header files
set(HEADERS
    src/MainWindow.h
    src/VideoPlayer.h
    src/VideoRenderer.h
    src/FFmpegDecoder.h
    src/PlaylistWidget.h
    src/ControlBar.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
if(Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGL
        OpenGL::GL
        ${FFMPEG_LIBRARIES}
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::OpenGL
        OpenGL::GL
        ${FFMPEG_LIBRARIES}
    )
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${FFMPEG_INCLUDE_DIRS}
)

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE
    ${FFMPEG_CFLAGS_OTHER}
)

# Link directories
target_link_directories(${PROJECT_NAME} PRIVATE
    ${FFMPEG_LIBRARY_DIRS}
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

if(APPLE)
    # macOS-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()
