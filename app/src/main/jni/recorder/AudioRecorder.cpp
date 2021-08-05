//
// Created by xu on 21-7-28.
//

#include "include/AudioRecorder.h"
#include <Log.h>


extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavcodec/codec_id.h>
    #include <libswresample/swresample.h>
    #include <libavutil/avassert.h>
    #include <libavutil/opt.h>
    #include <unistd.h>
}

AudioRecorder:: AudioRecorder(const char *outputUrl, int sampleRate,
        int channelLayout, int sampleFormat) {
        Log::d(outputUrl);
        strcpy(m_outputUrl,outputUrl);
        m_sampleRate = sampleRate;
        m_sampleFormat = sampleFormat;
        m_channelLayout = channelLayout;
}

AudioRecorder::~AudioRecorder() {}

int AudioRecorder::onFrameEncode(AudioFrame *frame) {
    if(m_exit)return 0;
    AudioFrame * audioFrame = new AudioFrame(frame->data,frame->dataSize,true);
    m_queue.push(audioFrame);
    return 0;
}

int AudioRecorder::stopRecord() {
    Log::d("stop record");
    m_exit = 1;
    if(m_encodeThread != nullptr){
        m_encodeThread->join();
        delete m_encodeThread;
        m_encodeThread = nullptr;
        int result = encodeFrame(nullptr);
        if(result >= 0){
            av_write_trailer(m_formatContext);
        }
    }

    while (!m_queue.isEmpty()){
        AudioFrame* audioFrame = m_queue.pop();
        delete audioFrame;
    }

    if(m_swsContext != nullptr){
        swr_close(m_swsContext);
        swr_free(&m_swsContext);
        m_swsContext = nullptr;
    }

    if(m_av_frame != nullptr){
        av_frame_free(&m_av_frame);
        m_av_frame = nullptr;
    }


    if(m_pFrameBuffer != nullptr){
        av_free(m_pFrameBuffer);
        m_pFrameBuffer = nullptr;
    }
    if(m_AvCodecContext != nullptr){
        avcodec_close(m_AvCodecContext);
        avcodec_free_context(&m_AvCodecContext);
        m_AvCodecContext = nullptr;
    }

    if(m_formatContext != nullptr){
        avio_close(m_formatContext->pb);
//        avformat_free_context(m_formatContext); 此时不能释放 formatContext
        m_formatContext = nullptr;
    }



    return 0;
}
int AudioRecorder::startRecord() {
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

    this->m_av_stream = avformat_new_stream(m_formatContext, nullptr);
    if(m_av_stream == nullptr){
        Log::d("failure to new stream");
        return -1;
    }

    AVOutputFormat* avOutputFormat = m_formatContext->oformat;
    this->m_av_codec = avcodec_find_encoder(avOutputFormat->audio_codec);

    if(this->m_av_codec == nullptr){
        Log::d("fail to find codec");
        return -1;
    }


    this->m_AvCodecContext = this->m_av_stream->codec;
    this->m_AvCodecContext->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    this->m_AvCodecContext->codec_id = AV_CODEC_ID_AAC;
    this->m_AvCodecContext->codec_type = AVMEDIA_TYPE_AUDIO;
    this->m_AvCodecContext->sample_fmt = AV_SAMPLE_FMT_FLTP;
    this->m_AvCodecContext->sample_rate = DEFAULT_AUDIO_SAMPLE_RATE;
    this->m_AvCodecContext->bit_rate = DEFAULT_AUDIO_BIT_RATE;
    this->m_AvCodecContext->channel_layout = DEFAULT_CHANNEL_LAYOUT;
    this->m_AvCodecContext->channels = av_get_channel_layout_nb_channels(
            m_AvCodecContext->channel_layout);
    result = avcodec_open2(m_AvCodecContext,m_av_codec, nullptr);
    if(result < 0){
        Log::d("can not open codec");
        return -1;
    }
    int streamIndex = m_av_stream->index;
    av_dump_format(m_formatContext,0,m_outputUrl,1);

    m_av_frame = av_frame_alloc();
    m_av_frame->nb_samples =m_AvCodecContext->frame_size;
    m_av_frame->format = m_AvCodecContext->sample_fmt;

    Log::d("StartRecord channels=%d ,m_frameSize=%d, format=%d", m_AvCodecContext->channels,
            m_AvCodecContext->frame_size,m_AvCodecContext->sample_fmt);
    m_frame_buffer_size = av_samples_get_buffer_size(nullptr,m_AvCodecContext->channels,
           m_AvCodecContext->frame_size, m_AvCodecContext->sample_fmt,1);
    m_pFrameBuffer = (uint8_t*)av_malloc(m_frame_buffer_size);

    avcodec_fill_audio_frame(m_av_frame,m_AvCodecContext->channels,m_AvCodecContext->sample_fmt,
            m_pFrameBuffer,m_frame_buffer_size,1);

    avformat_write_header(m_formatContext, nullptr);
    result = av_new_packet(&m_avPacket,m_frame_buffer_size);
    if(result < 0){
        Log::d("fail to new packet");
        return -1;
    }

    m_swsContext = swr_alloc();
    av_opt_set_channel_layout(m_swsContext,  "in_channel_layout", m_channelLayout, 0);
    av_opt_set_channel_layout(m_swsContext,  "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
    av_opt_set_int(m_swsContext,  "in_sample_rate", m_sampleRate, 0);
    av_opt_set_int(m_swsContext,  "out_sample_rate", DEFAULT_AUDIO_SAMPLE_RATE, 0);
    av_opt_set_sample_fmt(m_swsContext,  "in_sample_fmt", AVSampleFormat(m_sampleFormat), 0);
    av_opt_set_sample_fmt(m_swsContext,  "out_sample_fmt", AV_SAMPLE_FMT_FLTP, 0);
    swr_init(m_swsContext);


    if(result >= 0){
        m_encodeThread = new std::thread(StartACCEncoderThread,this);
    }
    return 0;
}


void AudioRecorder::StartACCEncoderThread(AudioRecorder *recorder) {
    while (!recorder->m_exit || !recorder->m_queue.isEmpty()){
        if(recorder->m_queue.isEmpty()){
            usleep(10*1000);
            continue;
        }

        AudioFrame* audioFrame = recorder->m_queue.pop();
        AVFrame *avFrame = recorder->m_av_frame;

        //??? audioFrame->dataSize/4 很奇怪
        int result = swr_convert(recorder->m_swsContext,avFrame->data,avFrame->nb_samples,
                                 (const uint8_t**)&(audioFrame->data),audioFrame->dataSize/4);
        Log::d("start aac encode thread");
        if(result > 0){
            avFrame->pts = recorder->m_frameIndex ++;
            recorder->encodeFrame(avFrame);
        }
        delete audioFrame;
    }
}

int AudioRecorder::encodeFrame(AVFrame *avFrame) {
    int result = avcodec_send_frame(m_AvCodecContext,avFrame);
    if(result < 0){
        Log::d("send av frame failure");
        return -1;
    }

    while (!result){
        result = avcodec_receive_packet(m_AvCodecContext,&m_avPacket);
        if(result == AVERROR(EAGAIN) || result == AVERROR_EOF){
            return 0;
        } else if(result < 0){
            Log::d("can not receive packet");
            return -1;
        }


        Log::d("SingleAudioRecorder::EncodeFrame frame pts=%ld, size=%d", m_avPacket.pts, m_avPacket.size);
        m_avPacket.stream_index = this->m_av_stream->index;
        av_interleaved_write_frame(m_formatContext,&m_avPacket);
        av_packet_unref(&m_avPacket);
    }

    return 0;
}