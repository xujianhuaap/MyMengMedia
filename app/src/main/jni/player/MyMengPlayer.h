//
// Created by xu on 21-9-30.
//

#ifndef MYMENGMEDIA_MYMENGPLAYER_H
#define MYMENGMEDIA_MYMENGPLAYER_H

#include <jni.h>
#include <AudioFrame.h>
#include "decoder/AudioDecoder.h"

#define JAVA_PLAYER_EVENT_CALLBACK_API_NAME "playerEventCallback"


class MyMengPlayer {
public:
    MyMengPlayer() {}

    ~MyMengPlayer() {}

    void Init(JNIEnv *env, jobject obj, const char *url);

    void UnInit();

    void Play();

    void Pause();

    void Stop();

    void SeekToPosition(float pos);

    long GetMediaParams(int paramType);


    static MyMengPlayer* getInstance(JNIEnv *env, jobject obj);
    static void StoreNativeHandle(JNIEnv *env, jobject obj,MyMengPlayer* handle);
private:
    JNIEnv *getEnv(bool *isAttach);

    jobject getJavaObject();

    JavaVM *getJVM();

    static void postMessage(void *context, int msgType, float msgCode);

    JavaVM *m_jvm = nullptr;
    jobject m_java_obj;
    AudioRender* m_audioRender = nullptr;
    AudioDecoder * m_audio_decoder = nullptr;



};

#endif //MYMENGMEDIA_MYMENGPLAYER_H
