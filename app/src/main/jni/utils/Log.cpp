//
// Created by xu on 21-7-28.
//

#include "include/Log.h"

void Log::d(const char *msg)  {
    LOGCATD("%s", msg);
}

void Log::e(const char *msg) {
    LOGCATE("%s",msg);
}

void Log::i(const char *msg) {
    LOGCATI("%s",msg);
}

void Log::v(const char *msg) {
    LOGCATV("%s", msg);
}