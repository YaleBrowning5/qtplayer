#include "VideoPlayer.h"
#include <QDebug>

VideoPlayer::VideoPlayer(QObject *parent)
    : QObject(parent)
    , decoder(nullptr)
    , frameTimer(nullptr)
    , decoderThread(nullptr)
    , state(Stopped)
    , playbackSpeed(1.0)
    , baseFps(25.0)
{
    decoder = std::make_unique<FFmpegDecoder>();
    
    frameTimer = new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, &VideoPlayer::onFrameTimer);
    
    connect(decoder.get(), &FFmpegDecoder::endOfStream, this, &VideoPlayer::onEndOfStream);
    connect(decoder.get(), &FFmpegDecoder::error, this, &VideoPlayer::errorOccurred);
}

VideoPlayer::~VideoPlayer() {
    stop();
}

bool VideoPlayer::loadVideo(const QString &filePath) {
    stop();
    
    if (!decoder->openFile(filePath)) {
        return false;
    }
    
    // Get video information
    int width = decoder->getWidth();
    int height = decoder->getHeight();
    baseFps = decoder->getFps();
    
    // Allocate frame buffer
    frameBuffer.resize(width * height * 4); // RGBA
    
    emit videoLoaded();
    emit durationChanged(decoder->getDuration());
    
    return true;
}

void VideoPlayer::closeVideo() {
    stop();
    decoder->closeFile();
    frameBuffer.clear();
}

void VideoPlayer::play() {
    if (state == Playing) {
        return;
    }
    
    state = Playing;
    updateTimerInterval();
    frameTimer->start();
    
    emit stateChanged(state);
}

void VideoPlayer::pause() {
    if (state != Playing) {
        return;
    }
    
    state = Paused;
    frameTimer->stop();
    
    emit stateChanged(state);
}

void VideoPlayer::stop() {
    if (state == Stopped) {
        return;
    }
    
    state = Stopped;
    frameTimer->stop();
    
    emit stateChanged(state);
}

void VideoPlayer::togglePlayPause() {
    if (state == Playing) {
        pause();
    } else {
        play();
    }
}

void VideoPlayer::seek(double position) {
    if (decoder->seek(position)) {
        emit positionChanged(position);
    }
}

void VideoPlayer::setPlaybackSpeed(double speed) {
    playbackSpeed = speed;
    if (state == Playing) {
        updateTimerInterval();
    }
}

int VideoPlayer::getVideoWidth() const {
    return decoder->getWidth();
}

int VideoPlayer::getVideoHeight() const {
    return decoder->getHeight();
}

double VideoPlayer::getDuration() const {
    return decoder->getDuration();
}

double VideoPlayer::getCurrentTime() const {
    return decoder->getCurrentTime();
}

double VideoPlayer::getFps() const {
    return baseFps;
}

void VideoPlayer::onFrameTimer() {
    if (state != Playing || frameBuffer.empty()) {
        return;
    }
    
    // Decode next frame
    if (decoder->decodeFrame(frameBuffer.data(), frameBuffer.size())) {
        int width = decoder->getWidth();
        int height = decoder->getHeight();
        
        emit frameReady(frameBuffer.data(), width, height);
        emit positionChanged(decoder->getCurrentTime());
    }
}

void VideoPlayer::onEndOfStream() {
    stop();
    emit playbackFinished();
}

void VideoPlayer::updateTimerInterval() {
    if (baseFps <= 0) {
        baseFps = 25.0;
    }
    
    // Calculate interval in milliseconds
    int interval = static_cast<int>((1000.0 / baseFps) / playbackSpeed);
    
    if (interval < 1) {
        interval = 1;
    }
    
    frameTimer->setInterval(interval);
}
