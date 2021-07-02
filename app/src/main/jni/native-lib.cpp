//
// Created by xu on 21-6-25.
//

#include <cn_skullmind_mbp_OSInfo.h>
#include <cn_skullmind_mbp_media_MediaPlayer.h>
#include "cn_skullmind_mbp_media_AudioCoder.h"

#include <libavutil/frame.h>
#include <libavutil/mem.h>

#include <libavformat/avformat.h>
#include <string>
#include "vector"

#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096
extern "C" {
    const std::string TAG = "ffmpeg";
    JNIEXPORT jstring JNICALL
    Java_cn_skullmind_mbp_OSInfo_getOSString(
            JNIEnv *env,
            jobject /* this */) {
        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

    JNIEXPORT jobject JNICALL Java_cn_skullmind_mbp_media_MediaPlayer_supportMediaFormats
            (JNIEnv *env, jobject) {
        std::vector<char,std::allocator<char>> formats = {"a","v"};
        return env->NewCharArray(formats.size());

    }
static int get_format_from_sample_fmt(const char **fmt,
                                      enum AVSampleFormat sample_fmt)
{
    int i;
    struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt; const char *fmt_be, *fmt_le;
    } sample_fmt_entries[] = {
            { AV_SAMPLE_FMT_U8,  "u8",    "u8"    },
            { AV_SAMPLE_FMT_S16, "s16be", "s16le" },
            { AV_SAMPLE_FMT_S32, "s32be", "s32le" },
            { AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
            { AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
    };
    *fmt = NULL;

    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
        struct sample_fmt_entry *entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt) {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }

    fprintf(stderr,
            "sample format %s is not supported as output format\n",
            av_get_sample_fmt_name(sample_fmt));
    return -1;
}

static void decode(AVCodecContext *dec_ctx, AVPacket *pkt, AVFrame *frame,
                   FILE *outfile)
{
    int i, ch;
    int ret, data_size;

    /* send the packet with the compressed data to the decoder */
    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error submitting the packet to the decoder\n");
        exit(1);
    }

    /* read all the output frames (in general there may be any number of them */
    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }
        data_size = av_get_bytes_per_sample(dec_ctx->sample_fmt);
        if (data_size < 0) {
            /* This should not occur, checking just for paranoia */
            fprintf(stderr, "Failed to calculate data size\n");
            exit(1);
        }
        for (i = 0; i < frame->nb_samples; i++)
            for (ch = 0; ch < dec_ctx->channels; ch++)
                fwrite(frame->data[ch] + data_size*i, 1, data_size, outfile);
    }
}


    JNIEXPORT jint JNICALL Java_cn_skullmind_mbp_media_AudioCoder_readSamples
        (JNIEnv *env, jobject){
        const char* fileName;
        const char* outFileName;


        AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_MP3);
        if(!codec){
            std::printf("ffmpeg -->  no coder can use");
            return -1;
        }
        AVCodecParserContext*  codecParserContext = av_parser_init(codec->id);
        if(!codecParserContext){
            std::printf("ffmpeg -->  no parser can use");
            return -1;
        }

        AVCodecContext* context = avcodec_alloc_context3(codec);
        if(!context){
            std::printf("ffmpeg -->  can not allocate codec context");
            return -1;
        }

        if(avcodec_open2(context,codec,NULL) < 0){
            std::printf("ffmpeg --> can not open codec");
            return -1;
        }

        FILE* openFile = fopen(fileName,"rb");
        if(!openFile){
            std::printf("ffmpeg --> can not open file %s",fileName);
            return -1;
        }

        FILE* outputFile = fopen(outFileName,"wb");
        if(!outputFile){
            av_free(context);
            return -1;

        }

        uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
        uint8_t * data = inbuf;
        size_t data_size = fread(inbuf,1,AUDIO_INBUF_SIZE,openFile);
        AVFrame*  decode_frame = NULL;
        AVPacket* packet ;
        int result;
        int len;
        while (data_size > 0){
            if(!decode_frame){
                if(!(decode_frame=av_frame_alloc())){
                    std::printf("ffmpeg --> can not allocate frame");
                    return -1;
                }
            }

            result = av_parser_parse2(codecParserContext,context,&packet->data,
                    &packet->size,data,data_size,AV_NOPTS_VALUE,AV_NOPTS_VALUE,0);
            if(result < 0){
                std::printf("ffmpeg --> parsing occur error");
                return -1;
            }
            data += result;
            data_size -=result;

            if(packet->size){
                decode(context,packet,decode_frame,outputFile);
            }

            if (data_size < AUDIO_REFILL_THRESH) {
                memmove(inbuf, data, data_size);
                data = inbuf;
                len = fread(data + data_size, 1,
                            AUDIO_INBUF_SIZE - data_size, openFile);
                if (len > 0)
                    data_size += len;
            }

        }
        return 0;
    }


}

