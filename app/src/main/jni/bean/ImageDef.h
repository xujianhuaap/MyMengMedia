//
// Created by xu on 21-7-29.
//

#ifndef MYMENGMEDIA_IMAGEDEF_H
#define MYMENGMEDIA_IMAGEDEF_H
#include <cstdint>
typedef struct _tag_NativeImage
{
    int width;
    int height;
    int format;
    uint8_t *ppPlane[3];
    int pLineSize[3];
    _tag_NativeImage()
    {
        width = 0;
        height = 0;
        format = 0;
        ppPlane[0] = nullptr;
        ppPlane[1] = nullptr;
        ppPlane[2] = nullptr;
        pLineSize[0] = 0;
        pLineSize[1] = 0;
        pLineSize[2] = 0;
    }
} NativeImage;
#endif //MYMENGMEDIA_IMAGEDEF_H
