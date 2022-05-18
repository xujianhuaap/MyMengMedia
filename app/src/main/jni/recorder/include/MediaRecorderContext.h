//
// Created by xu on 21-7-28.
//

#ifndef MYMENGMEDIA_MEDIARECORDERCONTEXT_H
#define MYMENGMEDIA_MEDIARECORDERCONTEXT_H
#include <mutex>
#include <jni.h>
#include "AudioRecorder.h"

#define MSG_TYPE_START 0
#define MSG_TYPE_END 1
#define MSG_TYPE_ERR 2
#define FUNCTION_MSG_CALL_BACK "onAudioStatus"

class MediaRecorderContext {
public:
    MediaRecorderContext();
    ~MediaRecorderContext();
    static void CreateMediaContext(JNIEnv *jniEnv, jobject instance);
    static void StoreMediaRecorderContext(JNIEnv *jniEnv,jobject instance, MediaRecorderContext* ctx);
    static void DeleteMediaRecorderContext(JNIEnv *jniEnv,jobject instance);
    static MediaRecorderContext* GetContext(JNIEnv *env,jobject thisObj);
    int startRecordAudio(const char* outputUrl);
    int stopRecord();
    void onAudioData(uint8_t* pData,int size);
    void onAudioStatus(int type, const char* msg);
    void init(JNIEnv *jniEnv, jobject instance);
    void unInit();
    JNIEnv* getEnv(bool * isAttach);

private:
    static jfieldID s_contextHandle;
    AudioRecorder* m_audio_recorder = nullptr;
    std::mutex m_mutex;
    jobject m_java_obj;
    JavaVM* m_jvm;
};


#endif //MYMENGMEDIA_MEDIARECORDERCONTEXT_H
