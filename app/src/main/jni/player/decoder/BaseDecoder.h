//
// Created by xu on 21-9-30.
//

#ifndef MYMENGMEDIA_BASEDECODER_H
#define MYMENGMEDIA_BASEDECODER_H

#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "Decoder.h"

#define MAX_PATH   2048
enum DecoderState {
    STATE_UNKNOWN,
    STATE_DECODING,
    STATE_PAUSE,
    STATE_STOP
};

enum DecoderMsg {
    MSG_DECODER_INIT_ERR,
    MSG_DECODER_READY,
    MSG_DECODER_DONE,
    MSG_REQUEST_RENDER,
    MSG_DECODING_TIME

};

class BaseDecoder : public Decoder {
public:
    BaseDecoder() {}

    virtual ~BaseDecoder() {};

    virtual void Start();

    virtual void Stop();

    virtual void Pause();

    virtual void SeekToPosition(float position);

    virtual float GetDurationForSeconds() {
        return m_duration * 1.0f / 1000;
    }

    virtual float GetCurrentPosition();

    virtual void ClearCache() {}

    virtual void SetMessageCallback(void *context, MessageCallback callback){
        m_msg_context = context;
        m_message_call_back = callback;
    }
    void SetAVSyncCallBack(void* context,AVSyncCallback callback){
        m_av_context = context;
        m_av_sync_call_back = callback;
    }

protected:

    virtual int init(const char* url,AVMediaType mediaType);
    virtual void unInit();
    virtual void onDecodeReady() = 0;
    virtual void onDecodeDone() = 0;
    virtual void onFrameAvailable(AVFrame* avFrame)= 0;
    AVCodecContext* getCodecContext(){
        return m_av_codec_context;
    }

    void* m_msg_context = nullptr;
    MessageCallback m_message_call_back= nullptr;

private:
    int initDecoder();
    void unInitDecoder();
    void startDecodingThread();
    void updateTimeStamp();
    long avSync();
    int decodePacketData();
    static void doAvDecoding(BaseDecoder* baseDecoder);

    AVSyncCallback m_av_sync_call_back;
    void* m_av_context;
    long m_duration;//单位是ms
    AVCodecContext* m_av_codec_context = nullptr;
    AVFormatContext* m_format_context = nullptr;
    AVCodec * m_av_codec = nullptr;
    AVPacket * m_packet_data = nullptr;
    AVFrame * m_av_frame = nullptr;
    AVMediaType m_media_type = AVMEDIA_TYPE_UNKNOWN;
    char m_url[MAX_PATH] = {0};
    long m_current_timestamp = 0;
    long m_start_timestamp = 0;
    int m_stream_index = -1;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::thread *m_thread = nullptr;
    volatile float m_seek_postion = 0;
    volatile bool m_seek_success = false;
    volatile int m_decoder_state =  STATE_UNKNOWN;

};

#endif //MYMENGMEDIA_BASEDECODER_H
