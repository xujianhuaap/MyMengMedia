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

    virtual void Init(JNIEnv *env, jobject obj, const char *url) =0;

    virtual void UnInit()=0;

    virtual void Play() =0;

    virtual void Pause()=0;

    virtual void Stop()=0;

    virtual void SeekToPosition(float pos)=0;

    virtual long GetMediaParams(int paramType)=0;


    static MyMengPlayer* getInstance(JNIEnv *env, jobject obj);
    static void StoreNativeHandle(JNIEnv *env, jobject obj,MyMengPlayer* handle);
    static void postMessage(void *context, int msgType, float msgCode);
protected:
    JNIEnv *getEnv(bool *isAttach);

    jobject getJavaObject();

    JavaVM *getJVM();

    JavaVM *m_jvm = nullptr;
    jobject m_java_obj;

};

class AudioPlayer: public MyMengPlayer{
public:
    AudioPlayer(){}
    ~AudioPlayer(){}
    void Init(JNIEnv *env, jobject obj, const char *url) override;

    void UnInit() override;

    void Play() override;

    void Pause() override;

    void Stop() override;

    void SeekToPosition(float pos) override;

    long GetMediaParams(int paramType) override;

private:
    AudioRender* m_audioRender = nullptr;
    AudioDecoder * m_audio_decoder = nullptr;
};
//class VideoPlayer : MyMengPlayer{
//
//};
#endif //MYMENGMEDIA_MYMENGPLAYER_H
