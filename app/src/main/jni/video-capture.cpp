//
// Created by xu on 21-7-21.
//
#include <cn_skullmind_mbp_media_VideoCapture.h>

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
}


extern "C" {
    JNIEXPORT jint JNICALL Java_cn_skullmind_mbp_media_VideoCapture_captureCameraData
            (JNIEnv *env, jobject thisObject,jobject outputFile) {

        jclass fileClazz= env->GetObjectClass(outputFile);
        jmethodID getParentPath = env->GetMethodID(fileClazz, "getParent", "()Ljava/lang/String;");
        jstring outputParentPath = static_cast<jstring>(env->CallObjectMethod(outputFile, getParentPath));
        const char *outFileParentPath = env->GetStringUTFChars(outputParentPath,
                                                           reinterpret_cast<jboolean *>(false));

        av_register_all();
        avformat_network_init();
        AVFormatContext* avFormatContext = avformat_alloc_context();

        avdevice_register_all();

        AVInputFormat *avInputFormat = av_find_input_format("video4linux2");
        if(avformat_open_input(&avFormatContext,"/dev/video0",avInputFormat,NULL) != 0){
            printf("can not open input stream /dev/videolinux2");
            return -1;
        }

        if(avformat_find_stream_info(avFormatContext,NULL) < 0){
            printf("can not find stream info");
            return  -1;
        }

        int streamIndex = -1;
        for(int i = 0;i<avFormatContext->nb_streams;i++){
            if(avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
                streamIndex = i;
                break;
            }
        }

        if(streamIndex == -1){
            printf("can not find stream for video");
            return -1;
        }

        AVCodecContext* avCodecContext = avFormatContext->streams[streamIndex]->codec;
        AVCodec* avCodec = avcodec_find_decoder(avCodecContext->codec_id);
        if(avCodec == NULL){
            printf("can find coder");
            return -1;
        }

        if(avcodec_open2(avCodecContext,avCodec,NULL) < 0){
            printf("can not open coder");
            return -1;
        }

        AVFrame* avFrame = av_frame_alloc();
        AVFrame* avFrameYUV = av_frame_alloc();
        int pictureSize = avpicture_get_size(AV_PIX_FMT_YUV420P, avCodecContext->width, avCodecContext->height);
        unsigned char *out_buffer=(unsigned char *)av_malloc(pictureSize);
        avpicture_fill((AVPicture *)avFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, avCodecContext->width, avCodecContext->height);

        #if OUTPUT_YUV420P
            const char *outPutFilePath = outFileParentPath +"/output.yuv";
            FILE *outputYUV = fopen(outPutFilePath,"wb+");
        #endif

        AVPacket *avPacket = av_packet_alloc();
        av_init_packet(avPacket);

        struct SwsContext *img_convert_ctx = sws_getContext(avCodecContext->width,
                avCodecContext->height,avCodecContext->pix_fmt,avCodecContext->width,
                avCodecContext->height,AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,
                NULL,NULL);

        int result = -1;
        int get_picture ;
        while(av_read_frame(avFormatContext,avPacket) >=0){
            if(avPacket->stream_index == streamIndex){
                result = avcodec_decode_video2(avCodecContext,avFrame,&get_picture,avPacket);
                if(result < 0){
                    printf("decode video error");
                    return -1;
                }

                if(get_picture){
                    sws_scale(img_convert_ctx, (const unsigned char* const*)avFrame->data, avFrame->linesize, 0, avCodecContext->height, avFrameYUV->data, avFrameYUV->linesize);
                    #if OUTPUT_YUV420P
                        int y_size=avCodecContext->width*avCodecContext->height;
                        fwrite(avFrameYUV->data[0],1,y_size,outputYUV);    //Y
                        fwrite(avFrameYUV->data[1],1,y_size/4,outputYUV);  //U
                        fwrite(avFrameYUV->data[2],1,y_size/4,outputYUV);  //V
                    #endif
                }

            }
            av_packet_unref(avPacket);

        }

        sws_freeContext(img_convert_ctx);
        #if OUTPUT_YUV420P
            fclose(outputYUV);
        #endif
            avcodec_close(avCodecContext);
            avformat_close_input(&avFormatContext);
            av_packet_free(&avPacket);
            av_frame_free(&avFrame);
            av_frame_free(&avFrameYUV);
            env->ReleaseStringUTFChars(outputParentPath,outFileParentPath);
        return 0;

    }


}