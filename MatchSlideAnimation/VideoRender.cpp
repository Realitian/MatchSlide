/**
 * User: Bruce Washington
 * Date: 11/4/20.
 *
 */
#include <stdint.h>
#include <GLFW/glfw3.h>
#include "VideoRender.h"
// fix temporary array error in c++1x
#ifdef av_err2str
#undef av_err2str
av_always_inline char* av_err2str(int errnum)
{
    static char str[AV_ERROR_MAX_STRING_SIZE];
    memset(str, 0, sizeof(str));
    return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}
#endif

#if (LIBAVFORMAT_VERSION_MAJOR >= 58)
#define AV_ALLOCATE_FRAME() av_frame_alloc()
#endif
std::mutex m1;
VideoRender::VideoRender(): vtim(106),atim(107) {
    //random values, are overwritten anyways
}
// initialize the app data structure
void VideoRender::initializeAppData(AppData *data) {
    data->fmt_ctx = NULL;
    data->stream_idx = -1;
    data->video_stream = NULL;
    data->codec_ctx = NULL;
    data->decoder = NULL;
    data->av_frame = NULL;
    data->gl_frame = NULL;
    data->conv_ctx = NULL;
    data->pCodecCtxOrig=NULL;
    data->pStream=NULL;
}

void VideoRender::clearAppData(AppData *data) {
    if (data->av_frame) av_free(data->av_frame);
    if (data->gl_frame) av_free(data->gl_frame);
    if (data->packet) av_free(data->packet);
    if (data->codec_ctx) avcodec_close(data->codec_ctx);
    if (data->fmt_ctx) avformat_free_context(data->fmt_ctx);
    av_frame_free(&data->av_frame);
    // Close the video file
    avformat_close_input(&data->fmt_ctx);

}

void VideoRender::openVideoFrame(AppData *data, std::string filename) {
    // initialize libav
    try {
        av_register_all();
        avformat_network_init();
        // Open the file using libavformat
        data->fmt_ctx = avformat_alloc_context();
        if (!data->fmt_ctx) {
            printf("Couldn't created AVFormatContext\n");
        }

        // open video
        if (avformat_open_input(&data->fmt_ctx, filename.c_str(), NULL, NULL) < 0) {
            std::cout << "failed to open input" << std::endl;
            clearAppData(data);
        }

        // find stream info
        if (avformat_find_stream_info(data->fmt_ctx, NULL) < 0) {
            std::cout << "failed to get stream info" << std::endl;
            clearAppData(data);

        }
        // dump debug info
         //  av_dump_format(data->fmt_ctx, 0, filename.c_str(), 0);
        // Find the first valid video stream inside the file
        AVCodecParameters* av_codec_params;
        AVCodec* av_codec;
        for (int i = 0; i < data->fmt_ctx->nb_streams; ++i) {
            av_codec_params = data->fmt_ctx->streams[i]->codecpar;
            av_codec = avcodec_find_decoder(av_codec_params->codec_id);
            if (!av_codec) {
                continue;
            }
            if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
                data->video_stream = reinterpret_cast<AVStream *>(i);
                data->time_base = data->fmt_ctx->streams[i]->time_base;
                break;
            }
        }

        // find the video stream
        for (unsigned int i = 0; i < data->fmt_ctx->nb_streams; ++i) {
            if (data->fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
                data->stream_idx = i;
                data->time_base = data->fmt_ctx->streams[i]->time_base;
                data->time_base = (AVRational){1, 90000};
                break;
            }
        }

        if (data->stream_idx == -1) {
            std::cout << "failed to find video stream" << std::endl;
            clearAppData(data);
        }

        // Set up a codec context for the decoder
        data->codec_ctx = avcodec_alloc_context3(av_codec);
        if (!data->codec_ctx) {
            printf("Couldn't create AVCodecContext\n");
        }
        if (avcodec_parameters_to_context(data->codec_ctx, av_codec_params) < 0) {
            printf("Couldn't initialize AVCodecContext\n");
        }
        if (avcodec_open2(data->codec_ctx, av_codec, NULL) < 0) {
            printf("Couldn't open codec\n");

        }

        data->av_frame = av_frame_alloc();
        if (!data->av_frame) {
            printf("Couldn't allocate AVFrame\n");
        }
        data->packet = av_packet_alloc();
        if (!data->packet) {
            printf("Couldn't allocate AVPacket\n");
        }
#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(55,28,1)
        //Alloca il frame video
        data->av_frame = av_frame_alloc();

        //Alloca una struct AVFrame
        data->gl_frame = av_frame_alloc();
#else
        data->av_frame = avcodec_alloc_frame();
        data->gl_frame = avcodec_alloc_frame();
#endif

        int size = av_image_get_buffer_size(AV_PIX_FMT_RGB0,
                                            data->codec_ctx->width, data->codec_ctx->height, 16);
        uint8_t *internal_buf = (uint8_t *)av_malloc(size * sizeof(uint8_t));
        av_image_fill_arrays(data->gl_frame->data, data->gl_frame->linesize, internal_buf,
                             AV_PIX_FMT_RGB0, data->codec_ctx->width, data->codec_ctx->height, 1);
        data->packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    }catch (int e) {
        std::vector<std::string> error_strings =
                {"Error opening file\n",
                 "Error reading stream information\n",
                 "no videostream found\n",
                 "unsupported codec\n",
                 "Couldn't copy codec context\n",
                 "Couldn't open codec\n",
                 "Couldn't allocate pFrameRGB\n",
                 "Couldn't find a proper audio decoder",
                 "Couldn't open the context with the decoder"};
        std::cerr << "EXCEPTION:\n" << error_strings[e];
        clearAppData(data);
    }
}

int64_t av_gettime(void)
 {
 #if HAVE_GETTIMEOFDAY
     struct timeval tv;
     gettimeofday(&tv, NULL);
     return (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
 #elif HAVE_GETSYSTEMTIMEASFILETIME
     FILETIME ft;
     int64_t t;
     GetSystemTimeAsFileTime(&ft);
     t = (int64_t)ft.dwHighDateTime << 32 | ft.dwLowDateTime;
     return t / 10 - 11644473600000000; /* Jan 1, 1601 */
 #else
     return -1;
 #endif
 }

bool VideoRender::readFrame(AppData *data) {
    try {
        // Decode one frame
        int response;

        while (av_read_frame(data->fmt_ctx, data->packet) >= 0) {
            if (reinterpret_cast<AVStream *>(data->packet->stream_index) not_eq (data->video_stream)) {
                av_packet_unref(data->packet);
                continue;
            }

            response = avcodec_send_packet(data->codec_ctx, data->packet);
            if (response < 0) {
                printf("Failed to decode packet: %s\n", av_err2str(response));
                return false;
            }

            response = avcodec_receive_frame(data->codec_ctx, data->av_frame);
           // response =  avcodec_receive_frame(data->codec_ctx, next_frame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
                av_packet_unref(data->packet);
                continue;
            } else if (response < 0) {
                printf("Failed to decode packet: %s\n", av_err2str(response));
                return false;
            }

            av_packet_unref(data->packet);
            break;
        }
        // Decode video frame
#pragma omp parallel
// Set up sws scaler
        if (!data->conv_ctx) {
            data->conv_ctx = sws_getContext(data->codec_ctx->width,
                                            data->codec_ctx->height, data->codec_ctx->pix_fmt,
                                            data->codec_ctx->width, data->codec_ctx->height,
                                            AV_PIX_FMT_RGB0,
                                            SWS_FAST_BILINEAR, NULL, NULL, NULL);
        }
        if (!data->conv_ctx) {
            printf("Couldn't initialize sw scaler\n");
            return false;
        }

        sws_scale(data->conv_ctx, data->av_frame->data, data->av_frame->linesize, 0,
                  data->codec_ctx->height, data->gl_frame->data, data->gl_frame->linesize);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, data->codec_ctx->width,
                        data->codec_ctx->height, GL_RGBA, GL_UNSIGNED_BYTE,
                        data->gl_frame->data[0]);
        int64_t dts = av_gettime(); // get current time in microseconds
        dts *= 9000;
        dts /= 1000000; // 1 second = 10^6 microseconds
        data->av_frame->pts = AV_NOPTS_VALUE; // is it okay?
        data->av_frame->pkt_dts = dts;

        return true;
    }catch (int e) {
        std::vector<std::string> error_strings =
                {"Error opening file\n",
                 "Error reading stream information\n",
                 "no videostream found\n",
                 "unsupported codec\n",
                 "Couldn't copy codec context\n",
                 "Couldn't open codec\n",
                 "Couldn't allocate pFrameRGB\n",
                 "Couldn't find a proper audio decoder",
                 "Couldn't open the context with the decoder"};
        std::cerr << "EXCEPTION:\n" << error_strings[e];
        clearAppData(data);
        return false;
    }
}

bool VideoRender::video_reader_seek_frame(AppData *data, int64_t ts) {
    try {
        do {
            av_seek_frame(data->fmt_ctx, data->stream_idx, ts, AVSEEK_FLAG_BACKWARD);
            // av_seek_frame takes effect after one frame, so I'm decoding one here
            // so that the next call to video_reader_read_frame() will give the correct
            // frame
            if (av_read_frame(data->fmt_ctx, data->packet) >= 0) {
                av_packet_unref(data->packet);
                continue;
            }
            if (data->packet->stream_index == data->stream_idx) {
                int frame_finished = 0;

                if (avcodec_decode_video2(data->codec_ctx, data->av_frame, &frame_finished,
                                          data->packet) < 0) {
                    av_packet_unref(data->packet);
                    continue;
                }
            }
            av_free_packet(data->packet);
        } while (data->packet->stream_index != data->stream_idx);
        return true;
    }catch (int e) {
        std::vector<std::string> error_strings =
                {"Error opening file\n",
                 "Error reading stream information\n",
                 "no videostream found\n",
                 "unsupported codec\n",
                 "Couldn't copy codec context\n",
                 "Couldn't open codec\n",
                 "Couldn't allocate pFrameRGB\n",
                 "Couldn't find a proper audio decoder",
                 "Couldn't open the context with the decoder"};
        std::cerr << "EXCEPTION:\n" << error_strings[e];
        clearAppData(data);
        return false;
    }
}
VideoRender::~VideoRender() {

}
