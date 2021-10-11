//
// Created by xu on 21-6-25.
//

#include <cn_skullmind_mbp_OSInfo.h>
#include <cn_skullmind_mbp_media_MediaPlayer.h>
#include "cn_skullmind_mbp_media_AudioCoder.h"
//TODO test
#include <OpenSLRender.cpp>
#include <MyMengPlayer.h>
#include "BaseDecoder.cpp"
#include <AudioDecoder.cpp>


#include <string>
#include "vector"


extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}


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
    std::vector<char, std::allocator<char>> formats = {"a", "v"};
    return env->NewCharArray(formats.size());

}


static int
decode_packet(AVCodecContext *avCodecContext, const AVPacket *pkt, AVFrame *frame, FILE *destFile) {
    int ret = 0;
    avcodec_decode_audio4(avCodecContext,frame, &ret, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error submitting a packet for decoding (%s)\n", av_err2str(ret));
    }

    return ret;
}


JNIEXPORT jint JNICALL Java_cn_skullmind_mbp_media_AudioCoder_generatePCMFile
        (JNIEnv *env, jobject thisObj, jobject sourceFileObj) {
    jclass fileClazz = env->GetObjectClass(sourceFileObj);

    jmethodID getFileName = env->GetMethodID(fileClazz, "getName", "()Ljava/lang/String;");
    jstring sourFileName = static_cast<jstring>(env->CallObjectMethod(sourceFileObj, getFileName));

    jmethodID getFilePath = env->GetMethodID(fileClazz, "getPath", "()Ljava/lang/String;");
    jstring sourFilePath = static_cast<jstring>(env->CallObjectMethod(sourceFileObj, getFilePath));

    jmethodID getParentPath = env->GetMethodID(fileClazz, "getParent", "()Ljava/lang/String;");
    jstring sourParentPath = static_cast<jstring>(env->CallObjectMethod(sourceFileObj,
                                                                        getParentPath));


    const char *inputFilePath = env->GetStringUTFChars(sourFilePath,
                                                       reinterpret_cast<jboolean *>(false));
    const char *parentFilePath = env->GetStringUTFChars(sourParentPath,
                                                        reinterpret_cast<jboolean *>(false));
    jclass clazz = env->GetObjectClass(thisObj);
    jmethodID funGetPCMFileName = env->GetMethodID(clazz, "getPCMFileName",
                                                   "(Ljava/lang/String;)Ljava/lang/String;");
    jstring outputStr = static_cast<jstring>((env)->CallObjectMethod(thisObj, funGetPCMFileName,
                                                                     sourFileName));
    const char *outFileName = env->GetStringUTFChars(outputStr,
                                                     reinterpret_cast<jboolean *>(false));
    std::printf("ffmpeg -->  no coder can use");

    av_register_all();
    AVFormatContext *avFormatContext = avformat_alloc_context();
    if (!avFormatContext) {
        std::printf("ffmpeg -->  no format context can use");
        return -1;
    }

    if (avformat_open_input(&avFormatContext, inputFilePath, NULL, NULL) != 0) {
        std::printf("ffmpeg -->  can not open file");;
        return -1;
    }

    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        std::printf("ffmpeg -->  av format info obtain error");
        return -1;
    }

    int audioStreamIndex = -1;
    for (int i = 0; i < avFormatContext->nb_streams; i++) {
        if (avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
            break;
        }
    }

    if (audioStreamIndex == -1) {
        std::printf("ffmpeg -->  has no audio stream");
    }


    AVCodecContext *context = avFormatContext->streams[audioStreamIndex]->codec;
    if (!context) {
        std::printf("ffmpeg -->  can not allocate codec context");
        return -1;
    }

    const AVCodec *codec = avcodec_find_decoder(context->codec_id);
    if (!codec) {
        std::printf("ffmpeg -->  no coder can use");
        return -1;
    }
    int result;
    result = avcodec_parameters_to_context(context,
                                           avFormatContext->streams[audioStreamIndex]->codecpar);
    if (result < 0) {
        std::printf("ffmpeg --> fail copy input parameter to output context");
    }
    if (avcodec_open2(context, codec, NULL) < 0) {
        std::printf("ffmpeg --> can not open codec");
        return -1;
    }


    //设置输入音频通道布局和输出音频通道布局
    int64_t in_layout = context->channel_layout;
    int64_t out_layout = AV_CH_LAYOUT_STEREO;
    //设置输入音频样本格式和输出音频样本格式
    AVSampleFormat in_sampleFormat = context->sample_fmt;
    AVSampleFormat out_sampleFormat = AV_SAMPLE_FMT_S16;
    //设置输入采样率和输出采样率
    int in_samplerate = context->sample_rate;
    int out_samplerate = 44100;

    //分配一个大小为44100*2的空间用在存储转码后的音频数据
    //奈奎斯特定理：采样频率为最高采样率*2时，可以完整保留原始信号中的信息
    uint8_t * outbuffer = (uint8_t *)av_malloc(44100 * 2);

    //得到通道数量
    int nb_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);


    SwrContext * swrContext = swr_alloc();
    //设置音频转换器上下文参数
    swr_alloc_set_opts(swrContext,
                       out_layout,out_sampleFormat,out_samplerate,
                       in_layout,in_sampleFormat,in_samplerate, 0, NULL);
    //初始化
    swr_init(swrContext);

    FILE *openFile = fopen(inputFilePath, "rb");
    if (!openFile) {
        std::printf("ffmpeg --> can not open file %s", inputFilePath);
        return -1;
    }

    std::string outputPath = std::string(parentFilePath) + "/" + std::string(outFileName);
    FILE *outputFile = fopen(outputPath.c_str(), "wb");
    if (!outputFile) {
        av_free(context);
        return -1;

    }

    av_dump_format(avFormatContext, audioStreamIndex, outputPath.c_str(), 0);


    AVFrame *decode_frame = av_frame_alloc();
    AVPacket *packet = av_packet_alloc();
    av_init_packet(packet);


    while (av_read_frame(avFormatContext, packet) >= 0) {
        // check if the packet belongs to a stream we are interested in, otherwise
        // skip it
        if (packet->stream_index == audioStreamIndex){
            result = decode_packet(context, packet, decode_frame, outputFile);
            if(result > 0){
                //转换成pcm
                swr_convert(swrContext,&outbuffer,44100 * 2,(const uint8_t **)decode_frame->data, decode_frame->nb_samples);
                //获取缓冲区大小
                int buffersize = av_samples_get_buffer_size(NULL,nb_channels, decode_frame->nb_samples,AV_SAMPLE_FMT_S16, 1);
                std::printf("BUFFERSIZE    === %d", buffersize);
                std::printf("OUTBUFFERSIZE === %d", strlen((const char*)outbuffer));

                fwrite(outbuffer, 1 ,buffersize, outputFile);
            }
        }

        av_packet_unref(packet);

        if (result < 0)
            break;
    }

    fflush(outputFile);
    fclose(outputFile);
    fclose(openFile);

    avformat_free_context(avFormatContext);
    avcodec_free_context(&context);
    av_frame_free(&decode_frame);
    av_packet_free(&packet);
    env->ReleaseStringUTFChars(sourFilePath,inputFilePath);
    env->ReleaseStringUTFChars(sourParentPath,parentFilePath);
    env->ReleaseStringUTFChars(outputStr,outFileName);
    return 0;
}


}

