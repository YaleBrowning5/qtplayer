#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <memory>
#include "FFmpegDecoder.h"

class VideoPlayer : public QObject {
    Q_OBJECT

public:
    enum PlaybackState {
        Stopped,
        Playing,
        Paused
    };

    explicit VideoPlayer(QObject *parent = nullptr);
    ~VideoPlayer();

    // File operations
    bool loadVideo(const QString &filePath);
    void closeVideo();

    // Playback control
    void play();
    void pause();
    void stop();
    void togglePlayPause();
    
    // Seek operation
    void seek(double position);
    
    // Playback speed
    void setPlaybackSpeed(double speed);
    double getPlaybackSpeed() const { return playbackSpeed; }
    
    // Get video information
    int getVideoWidth() const;
    int getVideoHeight() const;
    double getDuration() const;
    double getCurrentTime() const;
    double getFps() const;
    
    // Get current state
    PlaybackState getState() const { return state; }

signals:
    void stateChanged(PlaybackState state);
    void frameReady(const uint8_t *data, int width, int height);
    void positionChanged(double position);
    void durationChanged(double duration);
    void videoLoaded();
    void playbackFinished();
    void errorOccurred(const QString &error);

private slots:
    void onFrameTimer();
    void onEndOfStream();

private:
    std::unique_ptr<FFmpegDecoder> decoder;
    QTimer *frameTimer;
    QThread *decoderThread;
    
    PlaybackState state;
    double playbackSpeed;
    double baseFps;
    
    std::vector<uint8_t> frameBuffer;
    
    void updateTimerInterval();
};

#endif // VIDEOPLAYER_H
