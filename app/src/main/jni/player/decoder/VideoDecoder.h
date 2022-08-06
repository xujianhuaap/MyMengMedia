#ifndef MYMENGMEDIA_VIDEODECODER_H
#define MYMENGMEDIA_VIDEODECODER_H

#include <video/VideoRender.h>

extern "C" {
#include "libswscale/swscale.h"
};

class VideoDecoder : public BaseDecoder {
private:
    int m_video_width = 0;
    int m_video_height = 0;
    int m_render_width = 0;
    int m_render_height = 0;
    AVFrame *m_rgb_frame = nullptr;
    uint8_t *m_frame_buffer = nullptr;

    SwsContext *m_sws_context = nullptr;
    VideoRender *m_video_render = nullptr;


};

#endif