//
// Created by xu on 21-9-28.
//

#include "AudioGLRender.h"
AudioGLRender * AudioGLRender::GetInstance() {
    if(s_instance == nullptr){
        std::unique_lock<std::mutex>(s_mutex);
        s_instance = new AudioGLRender();
    }
    return s_instance;
}

void AudioGLRender::ReleaseInstance() {
    std::unique_lock<std::mutex> lock(s_mutex);
    if(s_instance != nullptr){
        delete s_instance;
        s_instance = nullptr;
    }

}

TODO