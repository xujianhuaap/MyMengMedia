//
// Created by xu on 21-7-28.
//

#include "include/Log.h"
#include <stdarg.h>
#include "android/log.h"

void Log::d(const char *format,...)  {
    va_list paraList;
    va_start(paraList,format);
    va_end(paraList);
    printF(ANDROID_LOG_DEBUG,format,paraList);
}

void Log::e(const char *format,...) {
    va_list paraList;
    va_start(paraList,format);
    va_end(paraList);
    printF(ANDROID_LOG_ERROR,format,paraList);
}

void Log::i(const char *format,...) {
    va_list paraList;
    va_start(paraList,format);
    va_end(paraList);
    printF(ANDROID_LOG_INFO,format,paraList);
}

void Log::v(const char *format,...) {
    va_list paraList;
    va_start(paraList,format);
    va_end(paraList);
    printF(ANDROID_LOG_VERBOSE,format,paraList);
}

/***
 *  %d int
 *  %ld long
 *  %f float
 *  %lf double
 * @param priority
 * @param format
 * @param paraList
 */
void Log::printF(android_LogPriority priority,const char *format,va_list paraList){
    __android_log_vprint(priority,LOG_TAG,format,paraList);
}

