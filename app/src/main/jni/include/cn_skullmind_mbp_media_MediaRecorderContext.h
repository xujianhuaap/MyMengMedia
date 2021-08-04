/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class cn_skullmind_mbp_media_MediaRecorderContext */

#ifndef _Included_cn_skullmind_mbp_media_MediaRecorderContext
#define _Included_cn_skullmind_mbp_media_MediaRecorderContext
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    createContext
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_createContext
  (JNIEnv *, jobject);

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    destroyContext
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_destroyContext
  (JNIEnv *, jobject);

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_init
  (JNIEnv *, jobject);

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    unInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_unInit
  (JNIEnv *, jobject);

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    startRecordAudio
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_startRecordAudio
  (JNIEnv *, jobject, jstring);

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    onAudioData
 * Signature: ([BI)V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_onAudioData
  (JNIEnv *, jobject, jbyteArray, jint);

/*
 * Class:     cn_skullmind_mbp_media_MediaRecorderContext
 * Method:    stopRecord
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaRecorderContext_stopRecord
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
