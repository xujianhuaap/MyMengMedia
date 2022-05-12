//
// Created by xu on 21-8-2.
//


#include <MediaRecorderContext.h>
#include "include/cn_skullmind_mbp_media_MediaRecorderContext.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    createContext
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_createContext
        (JNIEnv *env, jobject thisObj){
    MediaRecorderContext::CreateMediaContext(env,thisObj);
}

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    destroyContext
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_destroyContext
        (JNIEnv *env, jobject thisObj){
    MediaRecorderContext::DeleteMediaRecorderContext(env,thisObj);
}

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_init
        (JNIEnv *env, jobject thisObject){
//TODO
}

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    unInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_unInit
        (JNIEnv *env, jobject thisObj){
    //TODO
}

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    startRecordAudio
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_startRecordAudio
        (JNIEnv *env, jobject thisObj, jstring outputUrl){
    const char * out = env->GetStringUTFChars(outputUrl, nullptr);
    MediaRecorderContext* mediaRecorderContext = MediaRecorderContext::GetContext(env,thisObj);
    env->ReleaseStringUTFChars(outputUrl,out);
    if(mediaRecorderContext){
        jclass  clazz = env->GetObjectClass(thisObj);
        jmethodID  methodID = env->GetMethodID(clazz,"onAudioStatus","(I)V");

        jfieldID  fieldId = env->GetStaticFieldID(clazz,"STATUS_START","I");
        jint stausValue = env->GetStaticIntField(clazz,fieldId);

        env->CallVoidMethod(thisObj,methodID,stausValue);

        env->DeleteLocalRef(clazz);
        return mediaRecorderContext->startRecordAudio(out);
    }
    return 0;
}


/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    onAudioData
 * Signature: ([BI)V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_onAudioData
        (JNIEnv *env, jobject thisObj, jbyteArray data, jint len){
    int length = env->GetArrayLength(data);
    unsigned char* buf = new unsigned char[length];
    env->GetByteArrayRegion(data,0,length, reinterpret_cast<jbyte*>(buf));
    MediaRecorderContext* mediaRecorderContext = MediaRecorderContext::GetContext(env, thisObj);
    if(mediaRecorderContext)mediaRecorderContext->onAudioData(buf,length);
    delete buf;
}

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    stopRecord
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_stopRecord
        (JNIEnv *env, jobject thisObj){
    MediaRecorderContext* mediaRecorderContext = MediaRecorderContext::GetContext(env, thisObj);
    if(mediaRecorderContext){
        mediaRecorderContext->stopRecord();
    }
    jclass  clazz = env->GetObjectClass(thisObj);
    jmethodID  methodID = env->GetMethodID(clazz,"onAudioStatus","(I)V");

    jfieldID  fieldId = env->GetStaticFieldID(clazz,"STATUS_END","I");
    jint stausValue = env->GetStaticIntField(clazz,fieldId);

    env->CallVoidMethod(thisObj,methodID,stausValue);

    env->DeleteLocalRef(clazz);
}

#ifdef __cplusplus
}
#endif
