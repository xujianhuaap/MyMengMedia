//
// Created by xu on 21-10-14.
//
#include "include/cn_skullmind_mbp_media_MediaPlayer.h"
#include "player/MyMengPlayer.h"
#ifdef __cplusplus
extern "C" {
#endif



JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaPlayer_nativeInit
        (JNIEnv * env, jobject obj,jstring url){
    const char * outUrl = env->GetStringUTFChars(url, nullptr);
    MyMengPlayer* player = new MyMengPlayer();
    player->Init(env,obj,outUrl);
    MyMengPlayer::StoreNativeHandle(env,obj,player);
}
JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaPlayer_nativePlay
(JNIEnv * env, jobject obj){
    MyMengPlayer* player = MyMengPlayer::getInstance(env,obj);
    if(player){
        player->Play();
    }
}

JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaPlayer_nativePause
        (JNIEnv * env, jobject obj){
    MyMengPlayer* player = MyMengPlayer::getInstance(env,obj);
    if(player){
        player->Pause();
    }
}

JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaPlayer_nativeStop
        (JNIEnv *env, jobject obj){
    MyMengPlayer* player = MyMengPlayer::getInstance(env,obj);
    if(player){
        player->Stop();
    }
}


JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaPlayer_nativeSeekToPos
        (JNIEnv *env, jobject obj, jint pos){
    MyMengPlayer* player = MyMengPlayer::getInstance(env,obj);
    if(player){
        player->SeekToPosition(pos);
    }
}

JNIEXPORT void JNICALL Java_cn_skullmind_mbp_media_MediaPlayer_nativeUnInit
        (JNIEnv * env, jobject obj){
    MyMengPlayer* player = MyMengPlayer::getInstance(env,obj);
    if(player){
        player->UnInit();
    }
}
#ifdef __cplusplus
}
#endif