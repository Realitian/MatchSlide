/**
 * User: Bruce Washington
 * Date: 11/4/20.
 *
 */

#ifndef VIDEORENDER_H
#define VIDEORENDER_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavdevice/avdevice.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <sys/time.h>
}

#include <iostream>
#include <string>
#include <atomic>
#include "JUtils.h"
#include <thread>
#include <mutex>

// app data structure
typedef struct {
    AVFormatContext *fmt_ctx;
    int stream_idx;
    AVRational time_base;
    AVStream *video_stream;
    AVCodecContext *codec_ctx;
    AVCodecContext *pCodecCtxOrig;
    AVCodec *decoder;
    AVPacket *packet;
    AVFrame *av_frame;
    AVFrame *gl_frame;
    AVStream *pStream;
    struct SwsContext *conv_ctx;
} AppData;

class VideoRender {
public:
    VideoRender();
    ~VideoRender();
    void clearAppData(AppData *data);
    void openVideoFrame(AppData *data, std::string filename);
    bool readFrame(AppData *data);
    void initializeAppData(AppData *data);
    bool video_reader_seek_frame(AppData *data, int64_t ts);


private:
    J::TimedLoop vtim, atim;
};

#endif //RENDERVIDEO_VIDEORENDER_H
