//
// Created by xu on 21-7-29.
//

#ifndef MYMENGMEDIA_AUDIOFRAME_H
#define MYMENGMEDIA_AUDIOFRAME_H
#include "cstdint"
class AudioFrame{
public:
    AudioFrame(uint8_t *pData,int size, bool hardCopy);
    ~AudioFrame();
};
#endif //MYMENGMEDIA_AUDIOFRAME_H
