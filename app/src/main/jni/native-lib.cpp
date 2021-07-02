//
// Created by xu on 21-6-25.
//

#include <cn_skullmind_mbp_OSInfo.h>
#include <cn_skullmind_mbp_media_MediaPlayer.h>
#include <libavformat/avformat.h>
#include <string>

extern "C" {

    JNIEXPORT jstring JNICALL
    Java_cn_skullmind_mbp_OSInfo_getOSString(
            JNIEnv *env,
            jobject /* this */) {
        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

    JNIEXPORT jobject JNICALL Java_cn_skullmind_mbp_media_MediaPlayer_supportMediaFormats
            (JNIEnv *, jobject) {
        av_register_all();

    }

}

