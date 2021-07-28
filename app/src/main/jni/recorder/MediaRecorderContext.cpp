//
// Created by xu on 21-7-28.
//

#include <Log.h>
#include <libavutil/samplefmt.h>
#include "include/MediaRecorderContext.h"

jfieldID MediaRecorderContext::s_contextHandle =0L;
MediaRecorderContext::MediaRecorderContext() {}
MediaRecorderContext::~MediaRecorderContext(){}

void MediaRecorderContext::CreateMediaContext(JNIEnv *jniEnv, jobject instance) {
    MediaRecorderContext* mediaRecorderContext = new MediaRecorderContext();
    StoreMediaRecorderContext(jniEnv,instance,mediaRecorderContext);
    Log::d("create media recorder context");
}

void MediaRecorderContext::StoreMediaRecorderContext(JNIEnv *jniEnv, jobject instance, MediaRecorderContext *ctx) {
    jclass recorderContextClazz = jniEnv->GetObjectClass(instance);
    if(recorderContextClazz == nullptr){
        Log::d("store media context class is null");
        return;
    }

    s_contextHandle = jniEnv->GetFieldID(recorderContextClazz,"nativeContextHandle","J");
    if(s_contextHandle == nullptr){
        Log::d("store media context nativeContextHandle is null");
        return;
    }

    jniEnv->SetLongField(instance,s_contextHandle, reinterpret_cast<jlong>(ctx));
}


void MediaRecorderContext::DeleteMediaRecorderContext(JNIEnv *jniEnv, jobject instance) {
    jclass recorderContextClazz = jniEnv->GetObjectClass(instance);
    if(s_contextHandle == nullptr){
        Log::d("DeleteMediaRecorderContext s_contextHandle is null");
        return;
    }

    MediaRecorderContext* recorderContext = reinterpret_cast<MediaRecorderContext*>(jniEnv->GetLongField(instance,s_contextHandle));
    if(recorderContext){
        delete recorderContext;
    }
    jniEnv->SetLongField(instance,s_contextHandle,0L);
}

int MediaRecorderContext::startRecordAudio(const char *outputUrl) {
    std::unique_lock<std::mutex> lock(m_mutex);
    if(m_audio_recorder == nullptr){
        m_audio_recorder = new AudioRecorder(outputUrl,DEFAULT_AUDIO_SAMPLE_RATE,DEFAULT_CHANNEL_LAYOUT,AV_SAMPLE_FMT_S16);
        m_audio_recorder->startRecord();
    }

    return 0;
}


int MediaRecorderContext::stopRecord() {
    if(m_audio_recorder != nullptr){
        m_audio_recorder->stopRecord();
        delete m_audio_recorder;
        m_audio_recorder = nullptr;
    }
    return 0;
}

void MediaRecorderContext::onAudioData(uint8_t *pData, int size) {
    AudioFrame audioFrame(pData,size, false);
    if(m_audio_recorder != nullptr){
        m_audio_recorder->onFrameEncode(&audioFrame);
    }
}