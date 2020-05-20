/**
 * User: Bruce Washington
 * Date: 11/4/20.
 *
 */

#ifndef VIDEORENDER_H
#define VIDEORENDER_H

#include "texture.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavdevice/avdevice.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
//#include <sys/time.h>
#include <ctime>
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
    int video_stream_idx = 0;
    AVRational time_base;
    AVStream *video_stream;
    AVCodecContext *video_codec_ctx;
    AVCodec *decoder;
    AVPacket packet;
    AVFrame *av_frame;
	AVFrame *gl_frame;
    AVStream *pStream;
	int video_width, video_height;
    AVPixelFormat pixFmt;

	SwsContext* sws_ctx;
} AppData;

class VideoRender {
public:
    VideoRender();
    ~VideoRender();
    void clearAppData(AppData *data);
    void openVideoFrame(AppData *data, std::string filename);
    void readFrame(AppData *data, Texture* texture);
	void decodeFrame(AppData *data, Texture* texture);
    void initializeAppData(AppData *data);
private:
    J::TimedLoop vtim, atim;
};

#endif //RENDERVIDEO_VIDEORENDER_H
