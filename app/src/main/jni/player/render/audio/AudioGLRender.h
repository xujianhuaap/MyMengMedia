//
// Created by xu on 21-9-28.
//

#ifndef MYMENGMEDIA_AUDIOGLRENDER_H
#define MYMENGMEDIA_AUDIOGLRENDER_H

#include <BaseGLRender.h>
#include <mutex>
#include "AudioFrame.h"
#include <GLES3/gl3.h>
#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>

#define MAX_SAMPLE_LEVEL 5000
#define RESAMPLE_LEVEL 40
class AudioGLRender{
public:
    static AudioGLRender* GetInstance();
    static void ReleaseInstance();
    void UpdateAudioFrame(AudioFrame* audioFrame);

//    virtual void OnSurfaceChanged(int w, int h);
//    virtual void OnSurfaceCreated();
//    virtual void OnDrawFrame();
//    virtual void UpdateMVPMatrix(TransformMatrix *pTransformMatrix);
//    virtual void UpdateMVPMatrix(int angleX, int angleY, float scaleX, float scaleY) ;
//    virtual void SetTouchLoc(float touchX, float touchY);

private:
    void init();
    void unInit();
    AudioGLRender(){
        init();
    }
    ~AudioGLRender(){
        unInit();
    }
//    void updateMesh();
    static AudioGLRender* s_instance;
    static std::mutex s_mutex;
    AudioFrame* m_audio_buffer= nullptr;
//    GLuint m_program_obj= 0;
//    GLuint m_vao_id;
//    GLuint m_vbo_ids[2];
//    glm::mat4 m_mvpmatrix;
//    glm::vec3* m_verticles_coords;
//    glm::vec2* m_texture_coords;
    int m_render_data_size;


};
#endif //MYMENGMEDIA_AUDIOGLRENDER_H
