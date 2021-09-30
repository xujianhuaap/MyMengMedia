//
// Created by xu on 21-9-28.
//

#include "AudioGLRender.h"
#include "Log.h"
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

void AudioGLRender::UpdateAudioFrame(AudioFrame *audioFrame) {
    Log::d("update audio frame");
    if(audioFrame == nullptr) return;
    std::unique_lock<std::mutex>lock(s_mutex);
    if(m_audio_buffer == nullptr){
        m_audio_buffer = new AudioFrame(audioFrame->data,audioFrame->dataSize);
        m_render_data_size = m_audio_buffer->dataSize/RESAMPLE_LEVEL;
    } else{
        memcpy(m_audio_buffer->data,audioFrame->data,audioFrame->dataSize);
    }
    lock.unlock();
}

void AudioGLRender::init() {
    m_audio_buffer = nullptr;
}

void AudioGLRender::unInit() {
    if(m_audio_buffer != nullptr){
        delete m_audio_buffer;
        m_audio_buffer = nullptr;
    }
}
//void AudioGLRender::OnSurfaceCreated() {
//    Log::d("AudioGLRender begin onSurfaceChanged");
//    if(m_program_obj) return;
//    char varshaderStr[]= "#version 300 es\n"
//                         "layout(location = 0) in vec4 a_position;\n"
//                         "layout(location = 1) in vec2 a_texCoord;\n"
//                         "uniform mat4 u_MVPMatrix;\n"
//                         "out vec2 v_texCoord;\n"
//                         "void main()\n"
//                         "{\n"
//                         "    gl_Position = u_MVPMatrix * a_position;\n"
//                         "    v_texCoord = a_texCoord;\n"
//                         "    gl_PointSize = 4.0f;\n"
//                         "}";
//    char fShaderStr[] =
//            "#version 300 es                                     \n"
//            "precision mediump float;                            \n"
//            "in vec2 v_texCoord;                                 \n"
//            "layout(location = 0) out vec4 outColor;             \n"
//            "uniform float drawType;                             \n"
//            "void main()                                         \n"
//            "{                                                   \n"
//            "  if(drawType == 1.0)                               \n"
//            "  {                                                 \n"
//            "      outColor = vec4(1.5 - v_texCoord.y, 0.3, 0.3, 1.0); \n"
//            "  }                                                 \n"
//            "  else if(drawType == 2.0)                          \n"
//            "  {                                                 \n"
//            "      outColor = vec4(1.0, 1.0, 1.0, 1.0);          \n"
//            "  }                                                 \n"
//            "  else if(drawType == 3.0)                          \n"
//            "  {                                                 \n"
//            "      outColor = vec4(0.3, 0.3, 0.3, 1.0);          \n"
//            "  }                                                 \n"
//            "}                                                   \n";
//
//    //TODO
//
//}
//void AudioGLRender::OnSurfaceChanged(int w, int h) {
//
//    //TODO
//
//}
