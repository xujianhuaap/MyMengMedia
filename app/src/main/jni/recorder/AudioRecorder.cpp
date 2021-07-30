//
// Created by xu on 21-7-28.
//

#include "include/AudioRecorder.h"
#include <Log.h>

extern "C" {
    #include <libavcodec/avcodec.h>
}

AudioRecorder::AudioRecorder(const char *outputUrl, int sampleRate,
        int channelLayout, int sampleFormat) {
        Log::d(std::strcat("audio recorder [outputUrl] -->",outputUrl));
        strcpy(m_outputUrl,outputUrl);
        m_sampleRate = sampleRate;
        m_sampleFormat = sampleFormat;
        m_channelLayout = channelLayout;
}

AudioRecorder::~AudioRecorder() {}

int AudioRecorder::onFrameEncode(AudioFrame *frame) {
    //TODO
    return 0;
}

int AudioRecorder::stopRecord() {
    //TODO
    return 0;
}
int AudioRecorder::startRecord() {
    Log::d("audio recorder start record");
    int result = -1;
    result = avformat_alloc_output_context2(&m_formatContext, nullptr,
            nullptr,m_outputUrl);
    if(result < 0){
        Log::d("AvFormatContext alloc failure");
        return -1;
    }
    result = avio_open(&m_formatContext->pb,m_outputUrl,AVIO_FLAG_READ_WRITE);

    if(result < 0){
        Log::d("av io open failure");
        return -1;
    }

    this->m_av_stream = avformat_new_stream(m_formatContext, m_formatContext->audio_codec);
    if(m_av_stream == nullptr){
        Log::d("failure to new stream");
        return -1;
    }

    AVOutputFormat* avOutputFormat = m_formatContext->oformat;
    this->m_av_codec = avcodec_find_decoder(avOutputFormat->audio_codec);

    if(this->m_av_codec == nullptr){
        Log::d("fail to find codec");
        return -1;
    }


    this->m_AvCodecContext = this->m_av_stream->codec;
    this->m_AvCodecContext->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    this->m_AvCodecContext->codec_id = AC_CODEC_ID_AAC;
    this->m_AvCodecContext->codec_type = AVMEDIA_TYPE_AUDIO;
    this->m_AvCodecContext->sample_fmt = AV_SAMPLE_FMT_FLTP;
    this->m_AvCodecContext->sample_rate = DEFAULT_AUDIO_SAMPLE_RATE;
    this->m_AvCodecContext->bit_rate = DEFAULT_AUDIO_BIT_RATE;
    this->m_AvCodecContext.

    return 0;
}