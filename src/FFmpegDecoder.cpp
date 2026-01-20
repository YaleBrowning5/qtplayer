#include "FFmpegDecoder.h"
#include <QDebug>

FFmpegDecoder::FFmpegDecoder(QObject *parent)
    : QObject(parent)
    , formatContext(nullptr)
    , codecContext(nullptr)
    , swsContext(nullptr)
    , frame(nullptr)
    , packet(nullptr)
    , videoStreamIndex(-1)
    , videoWidth(0)
    , videoHeight(0)
    , duration(0.0)
    , fps(0.0)
    , currentTime(0.0)
{
}

FFmpegDecoder::~FFmpegDecoder() {
    freeResources();
}

bool FFmpegDecoder::openFile(const QString &filePath) {
    QMutexLocker locker(&mutex);
    
    // Close any existing file
    freeResources();
    
    // Open input file
    if (avformat_open_input(&formatContext, filePath.toUtf8().constData(), nullptr, nullptr) < 0) {
        emit error("Could not open file: " + filePath);
        return false;
    }
    
    // Retrieve stream information
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        emit error("Could not find stream information");
        freeResources();
        return false;
    }
    
    // Find video stream
    videoStreamIndex = -1;
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }
    
    if (videoStreamIndex == -1) {
        emit error("Could not find video stream");
        freeResources();
        return false;
    }
    
    // Get codec parameters
    AVCodecParameters *codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    
    // Find decoder
    const AVCodec *codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        emit error("Unsupported codec");
        freeResources();
        return false;
    }
    
    // Allocate codec context
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        emit error("Could not allocate codec context");
        freeResources();
        return false;
    }
    
    // Copy codec parameters to context
    if (avcodec_parameters_to_context(codecContext, codecParams) < 0) {
        emit error("Could not copy codec parameters");
        freeResources();
        return false;
    }
    
    // Open codec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        emit error("Could not open codec");
        freeResources();
        return false;
    }
    
    // Get video properties
    videoWidth = codecContext->width;
    videoHeight = codecContext->height;
    
    // Calculate FPS
    AVRational timeBase = formatContext->streams[videoStreamIndex]->time_base;
    AVRational frameRate = formatContext->streams[videoStreamIndex]->avg_frame_rate;
    if (frameRate.num > 0 && frameRate.den > 0) {
        fps = av_q2d(frameRate);
    } else {
        fps = 25.0; // Default FPS
    }
    
    // Get duration
    if (formatContext->duration != AV_NOPTS_VALUE) {
        duration = formatContext->duration / (double)AV_TIME_BASE;
    } else {
        duration = 0.0;
    }
    
    // Allocate frame and packet
    frame = av_frame_alloc();
    packet = av_packet_alloc();
    
    if (!frame || !packet) {
        emit error("Could not allocate frame or packet");
        freeResources();
        return false;
    }
    
    // Initialize sws context for color conversion
    swsContext = sws_getContext(
        videoWidth, videoHeight, codecContext->pix_fmt,
        videoWidth, videoHeight, AV_PIX_FMT_RGBA,
        SWS_BILINEAR, nullptr, nullptr, nullptr
    );
    
    if (!swsContext) {
        emit error("Could not initialize swscale context");
        freeResources();
        return false;
    }
    
    currentTime = 0.0;
    
    return true;
}

void FFmpegDecoder::closeFile() {
    QMutexLocker locker(&mutex);
    freeResources();
}

bool FFmpegDecoder::decodeFrame(uint8_t *buffer, int bufferSize) {
    QMutexLocker locker(&mutex);
    
    if (!formatContext || !codecContext) {
        return false;
    }
    
    while (true) {
        // Read packet
        int ret = av_read_frame(formatContext, packet);
        if (ret < 0) {
            if (ret == AVERROR_EOF) {
                emit endOfStream();
            }
            return false;
        }
        
        // Check if packet belongs to video stream
        if (packet->stream_index != videoStreamIndex) {
            av_packet_unref(packet);
            continue;
        }
        
        // Send packet to decoder
        ret = avcodec_send_packet(codecContext, packet);
        if (ret < 0) {
            av_packet_unref(packet);
            continue;
        }
        
        // Receive frame from decoder
        ret = avcodec_receive_frame(codecContext, frame);
        av_packet_unref(packet);
        
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret < 0) {
            return false;
        }
        
        // Update current time
        if (frame->pts != AV_NOPTS_VALUE) {
            AVRational timeBase = formatContext->streams[videoStreamIndex]->time_base;
            currentTime = frame->pts * av_q2d(timeBase);
        }
        
        // Convert frame to RGBA
        uint8_t *destData[1] = { buffer };
        int destLinesize[1] = { videoWidth * 4 };
        
        sws_scale(
            swsContext,
            frame->data, frame->linesize,
            0, videoHeight,
            destData, destLinesize
        );
        
        emit frameDecoded();
        return true;
    }
}

bool FFmpegDecoder::seek(double timestamp) {
    QMutexLocker locker(&mutex);
    
    if (!formatContext || videoStreamIndex < 0) {
        return false;
    }
    
    // Convert timestamp to stream time base
    AVRational timeBase = formatContext->streams[videoStreamIndex]->time_base;
    int64_t seekTarget = timestamp / av_q2d(timeBase);
    
    // Seek to target position
    if (av_seek_frame(formatContext, videoStreamIndex, seekTarget, AVSEEK_FLAG_BACKWARD) < 0) {
        return false;
    }
    
    // Flush codec buffers
    avcodec_flush_buffers(codecContext);
    
    currentTime = timestamp;
    
    return true;
}

void FFmpegDecoder::freeResources() {
    if (swsContext) {
        sws_freeContext(swsContext);
        swsContext = nullptr;
    }
    
    if (frame) {
        av_frame_free(&frame);
    }
    
    if (packet) {
        av_packet_free(&packet);
    }
    
    if (codecContext) {
        avcodec_free_context(&codecContext);
    }
    
    if (formatContext) {
        avformat_close_input(&formatContext);
    }
    
    videoStreamIndex = -1;
    videoWidth = 0;
    videoHeight = 0;
    duration = 0.0;
    fps = 0.0;
    currentTime = 0.0;
}
