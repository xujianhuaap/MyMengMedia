//
// Created by xu on 21-7-28.
//

#ifndef MYMENGMEDIA_MEDIARECORDERCONTEXT_H
#define MYMENGMEDIA_MEDIARECORDERCONTEXT_H
#include <mutex>
#include <jni.h>
#include <libavutil/channel_layout.h>
#include "AudioRecorder.h"

#define DEFAULT_AUDIO_SAMPLE_RATE    44100
#define DEFAULT_CHANNEL_LAYOUT AV_CH_LAYOUT_STEREO

class MediaRecorderContext {
public:
    MediaRecorderContext();
    ~MediaRecorderContext();
    static void CreateMediaContext(JNIEnv *jniEnv, jobject instance);
    static void StoreMediaRecorderContext(JNIEnv *jniEnv,jobject instance, MediaRecorderContext* ctx);
    static void DeleteMediaRecorderContext(JNIEnv *jniEnv,jobject instance);
    int startRecordAudio(const char* outputUrl);
    int stopRecord();
    void onAudioData(uint8_t* pData,int size);

private:
    static jfieldID s_contextHandle;
    AudioRecorder* m_audio_recorder = nullptr;
    std::mutex m_mutex;
};


#endif //MYMENGMEDIA_MEDIARECORDERCONTEXT_H
