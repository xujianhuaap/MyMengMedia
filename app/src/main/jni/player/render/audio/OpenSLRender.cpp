//
// Created by xu on 21-9-28.
//

#include "OpenSLRender.h"
#include "AudioGLRender.h"
#include <Log.h>
#include <unistd.h>
#include "thread"

void OpenSLRender::init() {
    Log::d("OPenSLRender begin init");
    int result = -1;

    result = createEngine();
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender init  create engine failure %d",result);
        return;
    }

    result = createOutputMixer();
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create output mixer failure %d",result);
        return;
    }
    result = createAudioPlayer();
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create audio player failure %d",result);
        return;
    }

    m_thread = new std::thread(createSLWaitingThread,this);
    if (result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender init failure");
        unInit();
    } else {
        Log::d("OpenSLRender init success");
    }

}


void OpenSLRender::renderAudioFrame(uint8_t *pData, int data_size) {
    Log::d("player.render audio frame ，data size is  %d",data_size);
    if(m_audio_player_play){
        if(pData != nullptr && data_size > 0){
            while (getAudioFrameQueueSize()> MAX_QUEUE_BUFFER_SIZE && !m_exit){
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }

            std::unique_lock<std::mutex>lock(m_mutex);
            AudioFrame* audioFrame = new AudioFrame(pData,data_size);
            m_audio_queue.push(audioFrame);
            m_cond.notify_all();
            lock.unlock();
        }
    }
}


void OpenSLRender::unInit() {
    Log::d("OpenSLRender unInit");
    if(m_audio_player_play){
        (*m_audio_player_play)->SetPlayState(m_audio_player_play,SL_PLAYSTATE_STOPPED);
        m_audio_player_play = nullptr;
    }

    std::unique_lock<std::mutex>lock(m_mutex);
    m_exit = true;
    m_cond.notify_all();
    lock.unlock();

    if(m__audio_player_obj){
        (*m__audio_player_obj)->Destroy(m__audio_player_obj);
        m__audio_player_obj = nullptr;
        m_android_buffer_queue = nullptr;
    }

    if(m_output_mix_obj){
        (*m_output_mix_obj)->Destroy(m_output_mix_obj);
        m_output_mix_obj = nullptr;
    }

    if(m_engine_obj){
        (*m_engine_obj)->Destroy(m_engine_obj);
        m_engine_obj = nullptr;
        m_engine_engine = nullptr;
    }

    lock.lock();
    for(int i =0;i<m_audio_queue.size();i++){
        AudioFrame* audioFrame = m_audio_queue.front();
        m_audio_queue.pop();
        delete audioFrame;
    }
    lock.unlock();

    if(m_thread != nullptr){
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }

    AudioGLRender::ReleaseInstance();

}

int OpenSLRender::createEngine() {
    SLresult result = SL_RESULT_SUCCESS;
    result = slCreateEngine(&m_engine_obj,0, nullptr,0, nullptr, nullptr);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create engine failure");
        return result;

    }
    result = (*m_engine_obj)->Realize(m_engine_obj,SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create engine realize failure");
        return result;
    }

    result = (*m_engine_obj)->GetInterface(m_engine_obj,SL_IID_ENGINE,&m_engine_engine);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender get interface failure");
        return result;
    }
    return result;
}


int OpenSLRender::createOutputMixer() {
    Log::d("OpenSLRender begin create output mixer");
    SLresult result = SL_RESULT_SUCCESS;
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    result = (*m_engine_engine)->CreateOutputMix(m_engine_engine,&m_output_mix_obj,
            1, mids,mreq);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create output mix failure");
        return result;
    }

    result = (*m_output_mix_obj)->Realize(m_output_mix_obj,SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create output mix failure ");
        return result;
    }
    return result;
}


int OpenSLRender::createAudioPlayer() {
    Log::d("OPenSLRender begin create audio player");
    SLDataLocator_AndroidSimpleBufferQueue  queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,2};

    SLDataFormat_PCM  pcm = {SL_DATAFORMAT_PCM,
                             (SLint32)2,
                             SL_SAMPLINGRATE_44_1,
                             SL_PCMSAMPLEFORMAT_FIXED_16,
                             SL_PCMSAMPLEFORMAT_FIXED_16,
                             SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,
                             SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource slDataSource = {&queue,&pcm};
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX,m_output_mix_obj};
    SLDataSink slDataSink = {&outputMix, nullptr};

    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE,SL_IID_EFFECTSEND,SL_IID_VOLUME};
    const SLboolean req[3]={SL_BOOLEAN_TRUE,SL_BOOLEAN_TRUE,SL_BOOLEAN_TRUE};

    SLresult result = (*m_engine_engine)->CreateAudioPlayer(m_engine_engine,&m__audio_player_obj,
            &slDataSource,&slDataSink,3,ids,req);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create audio player failure");
        return result;
    }
    result = (*m__audio_player_obj)->Realize(m__audio_player_obj,SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OPenSLRender create audio player failure");
        return result;
    }

    result = (*m__audio_player_obj)->GetInterface(m__audio_player_obj,
                                                  SL_IID_PLAY,&m_audio_player_play);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create audio player failure for getting audio play");
        return result;
    }

    result = (*m__audio_player_obj)->GetInterface(m__audio_player_obj,
            SL_IID_BUFFERQUEUE,&m_android_buffer_queue);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create audio player failure for android buffer queue");
        return result;
    }

    result = (*m_android_buffer_queue)->RegisterCallback(m_android_buffer_queue,audioPlayerCallBack,this);

    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create audio player failure for register call back on the queue");
        return result;
    }

    result = (*m__audio_player_obj)->GetInterface(m__audio_player_obj,SL_IID_VOLUME,&m_audio_player_volume);
    if(result != SL_RESULT_SUCCESS){
        Log::d("OpenSLRender create audio player failure for volume");
        return result;
    }

    return result;
}


void OpenSLRender::startRender() {
    Log::d("OpenSLRender begin startRender");
    while (getAudioFrameQueueSize() < MAX_QUEUE_BUFFER_SIZE && !m_exit){
        std::unique_lock<std::mutex>lock(m_mutex);
        m_cond.wait_for(lock,std::chrono::milliseconds(15));
        lock.unlock();
    }

    (*m_audio_player_play)->SetPlayState(m_audio_player_play,SL_PLAYSTATE_PLAYING);
    audioPlayerCallBack(m_android_buffer_queue,this);
}


void OpenSLRender::handleAudioFrameQueue() {
    Log::d("OPenSLRender begin handle audio frame queue");
    if(m_audio_player_play == nullptr) return;
    while (getAudioFrameQueueSize() < MAX_QUEUE_BUFFER_SIZE &&!m_exit){
        std::unique_lock<std::mutex>lock(m_mutex);
        m_cond.wait_for(lock,std::chrono::milliseconds(15));
    }
    std::unique_lock<std::mutex>lock(m_mutex);
    AudioFrame* audioFrame = m_audio_queue.front();
    if(audioFrame != nullptr && m_audio_player_play){
        SLresult result = (*m_android_buffer_queue)->Enqueue(m_android_buffer_queue,audioFrame->data,(SLint32)audioFrame->dataSize);
        if(result == SL_RESULT_SUCCESS){
            AudioGLRender::GetInstance()->UpdateAudioFrame(audioFrame);
            m_audio_queue.pop();

            delete audioFrame;
        }
    }

    lock.unlock();
}


void OpenSLRender::createSLWaitingThread(OpenSLRender *openSlRender) {
    Log::d("OpenSLRender begin createSLWaitingThread");
    openSlRender->startRender();
}

void OpenSLRender::audioPlayerCallBack(SLAndroidSimpleBufferQueueItf bufferQueue, void *context) {
    OpenSLRender* render = static_cast<OpenSLRender*>(context);
    render->handleAudioFrameQueue();
}

int OpenSLRender::getAudioFrameQueueSize() {
    std::unique_lock<std::mutex>lock(m_mutex);
    return m_audio_queue.size();
}



void OpenSLRender::clearAudioCache() {
    std::unique_lock<std::mutex>lock(m_mutex);
    for(int i =0;i<getAudioFrameQueueSize();i++){

        AudioFrame* audioFrame = m_audio_queue.front();
        m_audio_queue.pop();
        delete audioFrame;
    }


}