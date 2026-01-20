#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , videoRenderer(nullptr)
    , videoPlayer(nullptr)
    , playlistWidget(nullptr)
    , controlBar(nullptr)
    , splitter(nullptr)
    , isFullscreen(false)
    , currentRotation(0)
{
    setupUI();
    setupMenus();
    setupConnections();
    
    // Mouse idle timer for fullscreen mode
    mouseIdleTimer = new QTimer(this);
    mouseIdleTimer->setInterval(3000); // 3 seconds
    connect(mouseIdleTimer, &QTimer::timeout, this, &MainWindow::onMouseIdleTimeout);
    
    setMouseTracking(true);
    
    resize(1280, 720);
    setWindowTitle("QtPlayer");
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Create splitter for playlist and video area
    splitter = new QSplitter(Qt::Horizontal, this);
    
    // Create playlist widget
    playlistWidget = new PlaylistWidget(this);
    playlistWidget->setMinimumWidth(200);
    playlistWidget->setMaximumWidth(400);
    
    // Create video renderer
    videoRenderer = new VideoRenderer(this);
    
    // Add to splitter
    splitter->addWidget(playlistWidget);
    splitter->addWidget(videoRenderer);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    
    // Create control bar
    controlBar = new ControlBar(this);
    
    // Add to main layout
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(controlBar);
    
    // Create video player
    videoPlayer = new VideoPlayer(this);
}

void MainWindow::setupMenus() {
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    // File menu
    fileMenu = menuBar->addMenu("&File");
    
    QAction *openFileAction = new QAction("&Open File...", this);
    openFileAction->setShortcut(QKeySequence::Open);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    fileMenu->addAction(openFileAction);
    
    QAction *openDirAction = new QAction("Open &Directory...", this);
    openDirAction->setShortcut(QKeySequence("Ctrl+D"));
    connect(openDirAction, &QAction::triggered, this, &MainWindow::onOpenDirectory);
    fileMenu->addAction(openDirAction);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExit);
    fileMenu->addAction(exitAction);
    
    // Help menu
    helpMenu = menuBar->addMenu("&Help");
    
    QAction *aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupConnections() {
    // Video player signals
    connect(videoPlayer, &VideoPlayer::videoLoaded, this, &MainWindow::onVideoLoaded);
    connect(videoPlayer, &VideoPlayer::frameReady, this, &MainWindow::onFrameReady);
    connect(videoPlayer, &VideoPlayer::positionChanged, this, &MainWindow::onPositionChanged);
    connect(videoPlayer, &VideoPlayer::durationChanged, this, &MainWindow::onDurationChanged);
    connect(videoPlayer, &VideoPlayer::playbackFinished, this, &MainWindow::onPlaybackFinished);
    connect(videoPlayer, &VideoPlayer::errorOccurred, this, &MainWindow::onErrorOccurred);
    
    // Control bar signals
    connect(controlBar, &ControlBar::playPauseClicked, this, &MainWindow::onPlayPauseClicked);
    connect(controlBar, &ControlBar::positionChanged, this, &MainWindow::onPositionChangeRequested);
    connect(controlBar, &ControlBar::speedChanged, this, &MainWindow::onSpeedChanged);
    connect(controlBar, &ControlBar::rotateClicked, this, &MainWindow::onRotateClicked);
    connect(controlBar, &ControlBar::fullscreenClicked, this, &MainWindow::onFullscreenClicked);
    
    // Playlist signals
    connect(playlistWidget, &PlaylistWidget::videoSelected, this, &MainWindow::onVideoSelected);
}

void MainWindow::onOpenFile() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open Video File",
        QString(),
        "Video Files (*.mp4 *.mkv *.avi *.mov *.wmv *.flv *.webm);;All Files (*.*)"
    );
    
    if (!filePath.isEmpty()) {
        if (!videoPlayer->loadVideo(filePath)) {
            QMessageBox::warning(this, "Error", "Failed to load video file.");
        }
    }
}

void MainWindow::onOpenDirectory() {
    QString dirPath = QFileDialog::getExistingDirectory(
        this,
        "Open Directory",
        QString(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!dirPath.isEmpty()) {
        playlistWidget->loadDirectory(dirPath);
    }
}

void MainWindow::onExit() {
    QApplication::quit();
}

void MainWindow::onAbout() {
    QMessageBox::about(
        this,
        "About QtPlayer",
        "<h2>QtPlayer</h2>"
        "<p>A feature-rich video player built with Qt, OpenGL, and FFmpeg.</p>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>Multiple video format support (MP4, MKV, AVI, etc.)</li>"
        "<li>OpenGL-accelerated rendering</li>"
        "<li>Video rotation (0°, 90°, 180°, 270°)</li>"
        "<li>Playback speed control</li>"
        "<li>Fullscreen mode with auto-hide controls</li>"
        "<li>Playlist management</li>"
        "</ul>"
        "<p>Version 1.0</p>"
    );
}

void MainWindow::onVideoLoaded() {
    int width = videoPlayer->getVideoWidth();
    int height = videoPlayer->getVideoHeight();
    videoRenderer->setVideoDimensions(width, height);
    
    // Reset rotation when loading new video
    currentRotation = 0;
    videoRenderer->setRotation(currentRotation);
    
    // Auto-play
    videoPlayer->play();
    controlBar->setPlayingState(true);
}

void MainWindow::onFrameReady(const uint8_t *data, int width, int height) {
    videoRenderer->updateFrame(data, width, height);
}

void MainWindow::onPositionChanged(double position) {
    controlBar->setPosition(position);
}

void MainWindow::onDurationChanged(double duration) {
    controlBar->setDuration(duration);
}

void MainWindow::onPlaybackFinished() {
    controlBar->setPlayingState(false);
}

void MainWindow::onErrorOccurred(const QString &error) {
    QMessageBox::warning(this, "Error", error);
}

void MainWindow::onPlayPauseClicked() {
    videoPlayer->togglePlayPause();
    controlBar->setPlayingState(videoPlayer->getState() == VideoPlayer::Playing);
}

void MainWindow::onPositionChangeRequested(double position) {
    videoPlayer->seek(position);
}

void MainWindow::onSpeedChanged(double speed) {
    videoPlayer->setPlaybackSpeed(speed);
}

void MainWindow::onRotateClicked() {
    currentRotation = (currentRotation + 90) % 360;
    videoRenderer->setRotation(currentRotation);
}

void MainWindow::onFullscreenClicked() {
    toggleFullscreen();
}

void MainWindow::onVideoSelected(const QString &filePath) {
    videoPlayer->stop();
    if (!videoPlayer->loadVideo(filePath)) {
        QMessageBox::warning(this, "Error", "Failed to load video: " + filePath);
    }
}

void MainWindow::onMouseIdleTimeout() {
    if (isFullscreen) {
        hideControls();
        setCursor(Qt::BlankCursor);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Space:
            onPlayPauseClicked();
            break;
        case Qt::Key_Escape:
            if (isFullscreen) {
                toggleFullscreen();
            }
            break;
        case Qt::Key_Left:
            // Rewind 5 seconds
            videoPlayer->seek(videoPlayer->getCurrentTime() - 5.0);
            break;
        case Qt::Key_Right:
            // Forward 5 seconds
            videoPlayer->seek(videoPlayer->getCurrentTime() + 5.0);
            break;
        case Qt::Key_F:
            toggleFullscreen();
            break;
        default:
            QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isFullscreen) {
        // Show controls when mouse moves
        showControls();
        setCursor(Qt::ArrowCursor);
        
        // Restart idle timer
        updateMouseIdleTimer();
        
        // Show/hide playlist based on mouse position
        if (event->pos().x() < 50) {
            playlistWidget->show();
            playlistWidget->setWindowOpacity(0.7);
        } else if (event->pos().x() > 250) {
            playlistWidget->hide();
        }
    }
    
    lastMousePos = event->pos();
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
}

void MainWindow::toggleFullscreen() {
    if (isFullscreen) {
        // Exit fullscreen
        showNormal();
        menuBar->show();
        playlistWidget->show();
        playlistWidget->setWindowOpacity(1.0);
        controlBar->show();
        setCursor(Qt::ArrowCursor);
        mouseIdleTimer->stop();
        isFullscreen = false;
    } else {
        // Enter fullscreen
        showFullScreen();
        menuBar->hide();
        playlistWidget->hide();
        controlBar->show();
        updateMouseIdleTimer();
        isFullscreen = true;
    }
}

void MainWindow::showControls() {
    if (isFullscreen) {
        controlBar->show();
    }
}

void MainWindow::hideControls() {
    if (isFullscreen) {
        controlBar->hide();
    }
}

void MainWindow::updateMouseIdleTimer() {
    mouseIdleTimer->stop();
    mouseIdleTimer->start();
}
