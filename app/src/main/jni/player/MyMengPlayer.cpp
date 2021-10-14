//
// Created by xu on 21-9-30.
//

#include <BaseGLRender.h>
#include <OpenSLRender.h>
#include "MyMengPlayer.h"

void MyMengPlayer::StoreNativeHandle(JNIEnv *env, jobject obj, MyMengPlayer *handle) {
    Log::d("MyMengPlayer::StoreNativeHandle");
    jclass clazz = env->GetObjectClass(obj);
    if(clazz == nullptr){
        Log::d("MyMengPlayer::StoreNativeHandle java obj is null");
        return;
    }

    jfieldID f = env->GetFieldID(clazz,"nativePlayerHandle","J");

    if(f == nullptr){
        Log::d("MyMengPlayer::StoreNativeHandle nativePlayerHandle field");
        return ;
    }

    env->SetLongField(obj,f, reinterpret_cast<jlong>(handle));

}
MyMengPlayer * MyMengPlayer::getInstance(JNIEnv *env, jobject obj) {
    Log::d("MyMengPlayer::getInstance");
    jclass clazz = env->GetObjectClass(obj);
    if(clazz == nullptr){
        Log::d("MyMengPlayer::getInstance java obj is null");
        return nullptr;
    }

    jfieldID f = env->GetFieldID(clazz,"nativePlayerHandle","J");

    if(f == nullptr){
        Log::d("MyMengPlayer::getInstance nativePlayerHandle field");
        return nullptr;
    }
    return reinterpret_cast<MyMengPlayer*>(env->GetLongField(obj,f));
}

void MyMengPlayer::Init(JNIEnv *env, jobject obj, const char *url) {
    env->GetJavaVM(&m_jvm);
    m_java_obj = env->NewGlobalRef(obj);
    m_audio_decoder = new AudioDecoder(url);
    m_audioRender = new OpenSLRender();


    m_audio_decoder->SetAudioRender(m_audioRender);
    m_audio_decoder->SetMessageCallback(this,postMessage);

}

void MyMengPlayer::UnInit() {
    if(m_audio_decoder){
        delete m_audio_decoder;
        m_audio_decoder = nullptr;
    }

    if(m_audioRender){
        delete m_audioRender;
        m_audioRender = nullptr;
    }
    bool isAttach = false;
   getEnv(&isAttach)->DeleteGlobalRef(m_java_obj);
   if(isAttach){
       getJVM()->DetachCurrentThread();
   }
}

void MyMengPlayer::Play() {
    if(m_audio_decoder){
        m_audio_decoder->Start();
    }
}

void MyMengPlayer::Pause() {
    if(m_audio_decoder){
        m_audio_decoder->Pause();
    }
}

void MyMengPlayer::Stop() {
    if(m_audio_decoder){
        m_audio_decoder->Stop();
    }
}

void MyMengPlayer::SeekToPosition(float pos) {
    if(m_audio_decoder){
        m_audio_decoder->SeekToPosition(pos);
    }
}

long MyMengPlayer::GetMediaParams(int paramType) {
    //TODO
    return 0;
}

JNIEnv * MyMengPlayer::getEnv(bool *isAttach) {
    JNIEnv *env;
    int status;
    if(m_jvm == nullptr){
        return nullptr;
    }

    *isAttach = false;
    status = m_jvm->GetEnv((void **)&env,  JNI_VERSION_1_4);
    if(status != JNI_OK){
        status = m_jvm->AttachCurrentThread(&env, nullptr);

        if(status != JNI_OK){
            return nullptr;
        }

        *isAttach = true;

    }

    return env;
}


JavaVM * MyMengPlayer::getJVM() {
   return m_jvm;
}

jobject MyMengPlayer::getJavaObject() {
    return m_java_obj;
}

void MyMengPlayer::postMessage(void *context, int msgType, float msgCode) {
    if(context){
        MyMengPlayer* player = static_cast<MyMengPlayer*>(context);
        bool isAttach = false;
        JNIEnv* env = player->getEnv(&isAttach);
        if(env == nullptr) return;

        jobject  javaObj = player->getJavaObject();
        jclass clazz = env->GetObjectClass(javaObj);
        jmethodID methodId = env->GetMethodID(clazz,JAVA_PLAYER_EVENT_CALLBACK_API_NAME,
                "(IF)V");
        env->CallVoidMethod(javaObj,methodId,msgType,msgCode);
        if(isAttach){
            player->getJVM()->DetachCurrentThread();
        }
    }
}