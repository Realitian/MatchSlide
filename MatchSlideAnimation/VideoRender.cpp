/**
 * User: Bruce Washington
 * Date: 11/4/20.
 *
 */
#include <stdint.h>
#include <Windows.h>
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
    data->video_stream_idx = 0;
    data->video_stream = NULL;
    data->video_codec_ctx = NULL;
    data->decoder = NULL;
    data->av_frame = NULL;
    data->video_codec_ctx = NULL;
    data->pStream=NULL;
}

void VideoRender::clearAppData(AppData *data) {
    if (data->av_frame) av_free(data->av_frame);
    if (data->video_codec_ctx) avcodec_close(data->video_codec_ctx);
    if (data->fmt_ctx) avformat_free_context(data->fmt_ctx);
    //av_frame_free(&data->av_frame);
    //avformat_close_input(&data->fmt_ctx);
	//avcodec_free_context(&data->video_codec_ctx);
}

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename)
{
    FILE *f;
    int i;

    f = fopen(filename,"w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 65535);
	//fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

	//for (i = 0; i < ysize; i++) {
	//	for (int xi = 0; xi < xsize; xi++) {
	//		fwrite(buf + i * wrap + 2*xi, 1, 1, f);
	//		fwrite(buf + i * wrap + 2*xi + 1, 1, 1, f);
	//	}
	//	//fwrite(buf + i * wrap, 1, wrap, f);
	//}

	//for (i = 0; i < ysize; i++)
	//{
	//	for (int xi = 0; xi < xsize; xi++)
	//	{
	//		fwrite(buf + i*wrap + 2*xi+1, 1, 1, f);
	//		fwrite(buf + i*wrap + 2*xi, 1, 1, f);

	//		//fwrite(buf + i*wrap + xi, 1, 1, f);
	//	}
	//}

	std::cout << "xsize : " << xsize << ", ysize : " << ysize << ", wrap : " << wrap << std::endl;
	
	for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, wrap, f);
		
    fclose(f);
}

/* Enable or disable frame reference counting. You are not supposed to support
 * both paths in your application but pick the one most appropriate to your
 * needs. Look for the use of refcount in this example to see what are the
 * differences of API usage between them. */
static int gRefCount = 0;

static int open_codec_context(int &streamIndex
                              , AVCodecContext **decCtx
                              , AVFormatContext *fmtCtx
                              , enum AVMediaType type)
{
    int ret;
    int index;
    AVStream *st;
    AVCodec *codec = nullptr;
    AVDictionary *opts = nullptr;
    ret = av_find_best_stream(fmtCtx, type, -1, -1, nullptr, 0);
    if (ret < 0)
    {
        std::cout << "Could not find stream " << av_get_media_type_string(type);
        return ret;
    }
    index = ret;
    st = fmtCtx->streams[index];
    codec = avcodec_find_decoder(st->codecpar->codec_id);
    if (!codec)
    {
        std::cout << "Cound not find codec " << av_get_media_type_string(type);
        return AVERROR(EINVAL);
    }
    *decCtx = avcodec_alloc_context3(codec);
    if (!*decCtx)
    {
        std::cout << "Failed to allocate codec context " << av_get_media_type_string(type);
        return AVERROR(ENOMEM);
    }
    ret = avcodec_parameters_to_context(*decCtx, st->codecpar);
    if (ret < 0)
    {
        std::cout << "Failed to copy codec parameters to decoder context" << av_get_media_type_string(type);
        return ret;
    }
    av_dict_set(&opts, "refcounted_frames", gRefCount ? "1" : "0", 0);

    ret = avcodec_open2(*decCtx, codec, &opts);
    if (ret < 0)
    {
        std::cout << "Failed to open codec " << av_get_media_type_string(type);
        return ret;
    }
    streamIndex = index;
    return 0;
}

void VideoRender::openVideoFrame(AppData *data, std::string filename) {
    // initialize libav
	avformat_network_init();

	int ret = 0;
    ret = avformat_open_input(&data->fmt_ctx, filename.c_str(), nullptr, nullptr);
    if ( 0 > ret)
    {
        return;
    }
    ret = avformat_find_stream_info(data->fmt_ctx, nullptr);
    if (0 > ret)
    {
        return;
    }
    ret = open_codec_context(data->video_stream_idx, &data->video_codec_ctx, data->fmt_ctx, AVMEDIA_TYPE_VIDEO);
    if (ret < 0)
    {
        std::cout << "open_codec_context failed" << std::endl;
        return;
    }
    data->video_stream = data->fmt_ctx->streams[data->video_stream_idx];
    data->video_width = data->video_codec_ctx->width;
    data->video_height = data->video_codec_ctx->height;
    data->pixFmt = data->video_codec_ctx->pix_fmt;

    av_dump_format(data->fmt_ctx, 0, filename.c_str(), 0);
    
	data->av_frame = av_frame_alloc();
	
	data->gl_frame = av_frame_alloc();
	uint8_t           *buffer = NULL;
	int numBytes = avpicture_get_size(AV_PIX_FMT_RGBA, data->video_codec_ctx->width, data->video_codec_ctx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	avpicture_fill((AVPicture *)data->gl_frame, buffer, AV_PIX_FMT_RGBA, data->video_codec_ctx->width, data->video_codec_ctx->height);

	data->sws_ctx = sws_getContext(	data->video_codec_ctx->width, data->video_codec_ctx->height,
                  data->video_codec_ctx->pix_fmt, data->video_codec_ctx->width, 
                  data->video_codec_ctx->height, AV_PIX_FMT_RGBA, SWS_BICUBIC,
                  NULL, NULL, NULL);
}

void VideoRender::readFrame(AppData *data, Texture* texture)
{
    av_init_packet(&data->packet);
    data->packet.data = nullptr;
    data->packet.size = 0;
    if(av_read_frame(data->fmt_ctx, &data->packet) >= 0)
    {
        if (data->packet.stream_index == data->video_stream_idx)
        {
            if (avcodec_send_packet(data->video_codec_ctx, &data->packet) == 0)
            {
                while (avcodec_receive_frame(data->video_codec_ctx, data->av_frame) == 0)
                {
                    decodeFrame(data, texture);
                }
            }
        }
    }

    /*if (avcodec_send_packet(data->video_codec_ctx, nullptr) == 0)
    {
        while (avcodec_receive_frame(data->video_codec_ctx, data->av_frame) == 0)
        {
            decodeFrame(data);
        }
    }*/
}

void VideoRender::decodeFrame(AppData *data, Texture* texture)
{
	int height_scaled = sws_scale(data->sws_ctx, (uint8_t const * const *)data->av_frame->data,
          data->av_frame->linesize, 0, data->video_codec_ctx->height,
          data->gl_frame->data, data->gl_frame->linesize);

    switch (data->av_frame->format) {
        case AV_PIX_FMT_YUV420P:
        {
			/*textureY->Update(data->av_frame->linesize[0],
				data->av_frame->height,
				data->av_frame->data[0]);	

			textureU->Update(data->av_frame->linesize[1],
				data->av_frame->height/2,
				data->av_frame->data[1]);	

			textureV->Update(data->av_frame->linesize[2],
				data->av_frame->height/2,
				data->av_frame->data[2]);	*/

            break;
        }
		case AV_PIX_FMT_YUVA444P12LE:
		{
			
			break;
		}
        case AV_PIX_FMT_NV12:
        {
            break;
        }
        default:
            //TODO support other format
            break;
    }

	texture->Update(data->av_frame->width,
				data->av_frame->height,
				data->gl_frame->data[0]);
}

VideoRender::~VideoRender() {	
    
}
