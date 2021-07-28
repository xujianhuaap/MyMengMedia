//
// Created by xu on 21-7-28.
//

#ifndef MYMENGMEDIA_AUDIORECORDER_H
#define MYMENGMEDIA_AUDIORECORDER_H


#include <AudioRender.h>

class AudioRecorder {
public:
    AudioRecorder(const char* outputUrl,int sampleRate,int channelLayout,int sampleFormat);
    ~AudioRecorder();
    int startRecord();
    int onFrameEncode(AudioFrame* frame);
    int stopRecord();
};


#endif //MYMENGMEDIA_AUDIORECORDER_H
