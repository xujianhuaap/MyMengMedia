//
// Created by xu on 21-9-30.
//

#ifndef MYMENGMEDIA_AUDIODECODER_H
#define MYMENGMEDIA_AUDIODECODER_H

#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include "BaseDecoder.h"
#include <Log.h>

static const uint64_t AUDIO_DST_CHANNEL_LAYOUT = AV_CH_LAYOUT_STEREO;
// 音频编码采样率
static const int AUDIO_DST_SAMPLE_RATE = 44100;

static const AVSampleFormat AUDIO_DST_SAMPLE_FMT = AV_SAMPLE_FMT_S16;

// ACC音频一帧采样数
static const int ACC_NB_SAMPLES = 1024;

// 音频编码通道数
static const int AUDIO_DST_CHANNEL_COUNTS = 2;


class AudioDecoder: public BaseDecoder{
public:
    AudioDecoder(char* url){
        init(url,AVMEDIA_TYPE_AUDIO);
    }

    virtual ~AudioDecoder(){
        unInit();
    }

    void SetAudioRender(AudioRender* render){
        m_render = render;
    }

    static long GetAudioDecoderStampForAVSync(void* context);

private:
    virtual void onDecodeDone();
    virtual void onDecodeReady();
    virtual void onFrameAvailable(AVFrame *avFrame);
    virtual void ClearCache();


    AudioRender* m_render;
    SwrContext *m_swrContext;
    uint8_t *m_audio_out_buffer;
    int m_nb_sample_per_channel;
    int m_dst_frame_data_size;
};
#endif //MYMENGMEDIA_AUDIODECODER_H
