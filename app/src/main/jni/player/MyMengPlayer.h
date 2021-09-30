//
// Created by xu on 21-9-30.
//

#ifndef MYMENGMEDIA_MYMENGPLAYER_H
#define MYMENGMEDIA_MYMENGPLAYER_H

#include <jni.h>
#include <AudioFrame.h>

class MyMengPlayer {
public:
    MyMengPlayer() {}

    ~MyMengPlayer() {}

    void Init(JNIEnv *env, jobject obj, char *url, int renderType);

    void UnInit();

    void Play();

    void Pause();

    void Stop();

    void SeekToPosition(float pos);

    long GetMediaParams(int paramType);

private:
    JNIEnv *getEnv(bool *isAttach);

    jobject getJavaObject();

    JavaVM *getJVM();

    static void postMessage(void *context, int msgType, float msgCode);

    JavaVM *m_jvm = nullptr;
    jobject m_java_obj;
    AudioRender* m_audioRender = nullptr;


};

#endif //MYMENGMEDIA_MYMENGPLAYER_H
