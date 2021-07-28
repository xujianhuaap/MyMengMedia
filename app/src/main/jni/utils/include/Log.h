//
// Created by xu on 21-7-28.
//

#ifndef MYMENGMEDIA_LOG_H
#define MYMENGMEDIA_LOG_H

#include <string>
#include "android/log.h"
#define LOG_TAG "MyMengMedia"
#define  LOGCATE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGCATV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGCATD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGCATI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

class Log {
public:
    static void d(const char *msg);
    static void e(const char *msg);
    static void i(const char *msg);
    static void v(const char *msg);
};
#endif //MYMENGMEDIA_LOG_H
