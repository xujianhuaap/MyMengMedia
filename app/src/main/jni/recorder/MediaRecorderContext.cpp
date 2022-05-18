//
// Created by xu on 21-7-28.
//

#include <Log.h>
#include "include/MediaRecorderContext.h"

jfieldID MediaRecorderContext::s_contextHandle =0L;
MediaRecorderContext::MediaRecorderContext() {

}
MediaRecorderContext::~MediaRecorderContext(){
    unInit();
}

void MediaRecorderContext::init(JNIEnv *jniEnv, jobject instance) {
    jniEnv->GetJavaVM(&m_jvm);
    m_java_obj = jniEnv->NewGlobalRef(instance);
}

void MediaRecorderContext::unInit() {
    bool isAttach = false;
    JNIEnv *env = getEnv(&isAttach);
    env->DeleteGlobalRef(m_java_obj);
    if(isAttach){
        m_jvm->DetachCurrentThread();
    }
}
void MediaRecorderContext::CreateMediaContext(JNIEnv *jniEnv, jobject instance) {
    MediaRecorderContext* mediaRecorderContext = new MediaRecorderContext();
    mediaRecorderContext->init(jniEnv,instance);
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


MediaRecorderContext* MediaRecorderContext::GetContext(JNIEnv *env, jobject thisObj) {
    if(s_contextHandle == nullptr){
        Log::d("get context failure");
        return nullptr;
    }
    MediaRecorderContext* p = reinterpret_cast<MediaRecorderContext*>(env->GetLongField(thisObj,s_contextHandle));
    return p;
}
int MediaRecorderContext::startRecordAudio(const char *outputUrl) {
    int result = 0;
    std::unique_lock<std::mutex> lock(m_mutex);
    if(m_audio_recorder == nullptr){
        m_audio_recorder = new AudioRecorder(outputUrl,DEFAULT_AUDIO_SAMPLE_RATE,DEFAULT_CHANNEL_LAYOUT,AV_SAMPLE_FMT_S16);
    }
    result = m_audio_recorder->startRecord();
    if(result < 0){
        onAudioStatus(MSG_TYPE_ERR,"occur error when start record");
    } else{
        onAudioStatus(MSG_TYPE_START,"success start record");
    }
    return 0;
}


int MediaRecorderContext::stopRecord() {
    int result = 0;
    std::unique_lock<std::mutex> lock(m_mutex);
    if(m_audio_recorder != nullptr){
       result =  m_audio_recorder->stopRecord();

        delete m_audio_recorder;
        m_audio_recorder = nullptr;
    }
    if(result < 0){
        onAudioStatus(MSG_TYPE_ERR,"occur error when stop record");
    } else {
        onAudioStatus(MSG_TYPE_END,"success stop record");
    }
    return result;
}

void MediaRecorderContext::onAudioData(uint8_t *pData, int size) {
    AudioFrame audioFrame(pData,size, false);
    if(m_audio_recorder != nullptr){
        m_audio_recorder->onFrameEncode(&audioFrame);
    }
}

void MediaRecorderContext::onAudioStatus(int type, const char *msg) {
    bool isAttach = false;
    JNIEnv* env = this->getEnv(&isAttach);
    if(env == nullptr) return;
    jclass  clazz = env->GetObjectClass(m_java_obj);
    jstring msgInfo = env->NewStringUTF(msg);
    jmethodID jmethodId = env->GetMethodID(clazz,FUNCTION_MSG_CALL_BACK,"(ILjava/lang/String;)V");
    env->CallVoidMethod(m_java_obj, jmethodId, type, msgInfo);
    if(isAttach){
        this->m_jvm->DetachCurrentThread();
    }
}

JNIEnv * MediaRecorderContext::getEnv(bool * isAttach) {
    if(m_jvm == nullptr) return nullptr;
    JNIEnv* env;
    *isAttach = false;
    bool  status = m_jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
    if(status != JNI_OK){
        status = m_jvm->AttachCurrentThread(&env, nullptr);
        if(status != JNI_OK){
            return nullptr;
        }
        *isAttach = true;
    }

    return env;

}

