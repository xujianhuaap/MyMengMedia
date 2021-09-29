//
// Created by xu on 21-9-27.
//

#ifndef MYMENGMEDIA_OPENSLRENDER_H
#define MYMENGMEDIA_OPENSLRENDER_H
#include "cstdint"
#include "AudioFrame.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <mutex>
#include <queue>

#define MAX_QUEUE_BUFFER_SIZE 3
class OpenSLRender: public AudioRender{
public:
    OpenSLRender();
    ~OpenSLRender(){}
    virtual void init();
    virtual void clearAudioCache();
    virtual void renderAudioFrame(uint8_t *pData, int data_size);
    virtual void unInit();

private:
    int createEngine();
    int createOutputMixer();
    int createAudioPlayer();
    int getAudioFrameQueueSize();
    void startRender();
    void handleAudioFrameQueue();
    static void createSLWaitingThread(OpenSLRender *openSlRender);
    static void audioPlayerCallBack(SLAndroidSimpleBufferQueueItf bufferQueue,
            void *context);

    SLObjectItf  m_engine_obj = nullptr;
    SLEngineItf m_engine_engine = nullptr;
    SLObjectItf m_output_mix_obj = nullptr;
    SLObjectItf m__audio_player_obj = nullptr;
    SLPlayItf m_audio_player_play = nullptr;
    SLVolumeItf  m_audio_player_volume = nullptr;
    SLAndroidSimpleBufferQueueItf m_android_buffer_queue = nullptr;
    std::queue<AudioFrame*> m_audio_queue;
    std::thread * m_thread = nullptr;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    volatile bool m_exit = false;
};


#endif //MYMENGMEDIA_OPENSLRENDER_H
