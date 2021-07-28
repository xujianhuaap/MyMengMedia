//
// Created by xu on 21-7-28.
//

#ifndef MYMENGMEDIA_AUDIORENDER_H
#define MYMENGMEDIA_AUDIORENDER_H

#include <cstdint>

class AudioFrame{
public:
    AudioFrame(uint8_t *pData,int size, bool hardCopy);
    ~AudioFrame();
};
#endif //MYMENGMEDIA_AUDIORENDER_H
