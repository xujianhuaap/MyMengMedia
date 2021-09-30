//
// Created by xu on 21-9-30.
//

#include "BaseDecoder.h"

void BaseDecoder::Start() {
    if(m_thread == nullptr){
        startDecodingThread();
    } else{
        std::unique_lock<std::mutex>lock(m_mutex);
        m_decoder_state = STATE_DECODING;
        m_cond.notify_all();

    }
}

void BaseDecoder::Pause() {
    std::unique_lock<std::mutex>lock(m_mutex);
    m_decoder_state = STATE_PAUSE;
}

void BaseDecoder::Stop() {
    Log::d("BaseDecoder stop");
    std::unique_lock<std::mutex> lock(m_mutex);
    m_decoder_state = STATE_STOP;
    m_cond.notify_all();

}

void BaseDecoder::SeekToPosition(float position) {
    Log::d("BaseDecoder seekToPosition %f",position);
    std::unique_lock<std::mutex> lock(m_mutex);
    m_seek_postion = position;
    m_decoder_state = STATE_DECODING;
    m_cond.notify_all();
}

float BaseDecoder::GetCurrentPosition() {
    return m_current_timestamp;
}

int BaseDecoder::init(const char *url, AVMediaType mediaType) {
    Log::d("BaseDecoder init url -> %s, media type -> %d",url,mediaType);
    strcpy(m_url,url);
    m_media_type = mediaType;
    return 0;
}




