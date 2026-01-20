#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include <QString>
#include <QObject>
#include <QMutex>
#include <atomic>
#include <memory>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

class FFmpegDecoder : public QObject {
    Q_OBJECT

public:
    explicit FFmpegDecoder(QObject *parent = nullptr);
    ~FFmpegDecoder();

    // Open and close video file
    bool openFile(const QString &filePath);
    void closeFile();

    // Get video information
    int getWidth() const { return videoWidth; }
    int getHeight() const { return videoHeight; }
    double getDuration() const { return duration; }
    double getFps() const { return fps; }
    
    // Decode frame
    bool decodeFrame(uint8_t *buffer, int bufferSize);
    
    // Seek operation
    bool seek(double timestamp);
    
    // Get current position
    double getCurrentTime() const { return currentTime; }

signals:
    void frameDecoded();
    void endOfStream();
    void error(const QString &errorMsg);

private:
    AVFormatContext *formatContext;
    AVCodecContext *codecContext;
    SwsContext *swsContext;
    AVFrame *frame;
    AVPacket *packet;
    
    int videoStreamIndex;
    int videoWidth;
    int videoHeight;
    double duration;
    double fps;
    double currentTime;
    
    QMutex mutex;
    
    // Helper functions
    void freeResources();
};

#endif // FFMPEGDECODER_H
