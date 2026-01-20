#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "VideoRenderer.h"
#include "VideoPlayer.h"
#include "PlaylistWidget.h"
#include "ControlBar.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    // File menu actions
    void onOpenFile();
    void onOpenDirectory();
    void onExit();
    void onAbout();
    
    // Video player events
    void onVideoLoaded();
    void onFrameReady(const uint8_t *data, int width, int height);
    void onPositionChanged(double position);
    void onDurationChanged(double duration);
    void onPlaybackFinished();
    void onErrorOccurred(const QString &error);
    
    // Control bar events
    void onPlayPauseClicked();
    void onPositionChangeRequested(double position);
    void onSpeedChanged(double speed);
    void onRotateClicked();
    void onFullscreenClicked();
    
    // Playlist events
    void onVideoSelected(const QString &filePath);
    
    // Mouse idle timer
    void onMouseIdleTimeout();

private:
    // UI components
    VideoRenderer *videoRenderer;
    VideoPlayer *videoPlayer;
    PlaylistWidget *playlistWidget;
    ControlBar *controlBar;
    QSplitter *splitter;
    
    // Menu
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *helpMenu;
    
    // Fullscreen state
    bool isFullscreen;
    QTimer *mouseIdleTimer;
    QPoint lastMousePos;
    
    // Rotation state
    int currentRotation;
    
    // Helper functions
    void setupUI();
    void setupMenus();
    void setupConnections();
    void toggleFullscreen();
    void showControls();
    void hideControls();
    void updateMouseIdleTimer();
};

#endif // MAINWINDOW_H
