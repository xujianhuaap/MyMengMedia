//
// Created by xu on 2022/7/12.
//
extern "C" {
#include "ffmpeg.h"
}
#include "include/cn_skullmind_mbp_transcode_FFMpeg.h"
#include <jni.h>
#include "Log.h"


extern "C" {
JNIEXPORT jint JNICALL Java_cn_skullmind_mbp_transcode_FFMpeg_nativeCmd
        (JNIEnv *env, jobject thisObj, jobjectArray args) {
    int argc = env->GetArrayLength(args);
    char **argv = (char **) malloc(argc * sizeof(char *));
    for (int i = 0; i < argc; ++i) {
        jstring value = (jstring) env->GetObjectArrayElement(args, i);
        const char *tmpValue = env->GetStringUTFChars(value, 0);

        argv[i] = (char *) malloc(INPUT_SIZE);
        strcpy(argv[i],tmpValue);
        env->ReleaseStringUTFChars(value,tmpValue);
    }
    int value = run(argc, argv);
    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    free(argv);
    Log::d("transcode result %d",value);
    return value;
}

}

