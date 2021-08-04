//
// Created by xu on 21-7-28.
//

#ifndef MYMENGMEDIA_AUDIORECORDER_H
#define MYMENGMEDIA_AUDIORECORDER_H

#include <thread>
#include <AudioFrame.h>
#include <ThreadSafeQueue.h>
#include <ImageDef.h>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/packet.h>
    #include <libswresample/swresample.h>
}

#define DEFAULT_AUDIO_SAMPLE_RATE    44100
#define DEFAULT_AUDIO_BIT_RATE 96000
#define DEFAULT_CHANNEL_LAYOUT AV_CH_LAYOUT_STEREO
class AudioRecorder {
public:
     AudioRecorder(const char* outputUrl,int sampleRate,int channelLayout,int sampleFormat);
    ~AudioRecorder();
    int startRecord();
    int onFrameEncode(AudioFrame* frame);
    int stopRecord();

private:
    static void StartACCEncoderThread(AudioRecorder * recorder);
    int encodeFrame(AVFrame* avFrame);
private:
    ThreadSafeQueue<AudioFrame*> m_queue;
    char m_outputUrl[4096]={0};
    int m_frameIndex = 0;
    int m_sampleRate;
    int m_channelLayout;
    int m_sampleFormat;
    int m_frame_buffer_size;
    AVPacket m_avPacket;
    AVFrame *m_av_frame = nullptr;
    uint8_t *m_pFrameBuffer= nullptr;
    AVCodec *m_av_codec= nullptr;
    AVStream *m_av_stream= nullptr;
    AVCodecContext *m_AvCodecContext= nullptr;
    AVFormatContext *m_formatContext= nullptr;
    std::thread *m_encodeThread=nullptr;
    SwrContext *m_swsContext=nullptr;
    volatile int m_exit =0;

};


#endif //MYMENGMEDIA_AUDIORECORDER_H
