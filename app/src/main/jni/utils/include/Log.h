//
// Created by xu on 21-7-28.
//

#ifndef MYMENGMEDIA_LOG_H
#define MYMENGMEDIA_LOG_H

#include <string>
#include "android/log.h"
#define LOG_TAG "MyMengMedia"


class Log {
public:
    static void d(const char *format,...);
    static void e(const char *format,...);
    static void i(const char *format,...);
    static void v(const char *format,...);

private:
    static void printF(android_LogPriority priority,const char *format,va_list paraList);
};
#endif //MYMENGMEDIA_LOG_H
