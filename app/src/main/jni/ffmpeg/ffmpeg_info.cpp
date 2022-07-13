#include "libavutil/ffversion.h"
#include "string.h"
#include "config.h"

#include <string>
#include "sstream"

const int program_birth_year = 2000;

std::string getVersion(){
    std::string str = "===== FFMpeg Version is ";
    str.append(FFMPEG_VERSION)
    .append(" =====");
    return  str;
}

std::string getCopyRights(){
    std::ostringstream s;
    s <<"Copyright (c) "
        << program_birth_year << "-" << CONFIG_THIS_YEAR
        << " the FFmpeg developers";

    return s.str();
}

std::string getFFMpegInfo(){
    return getVersion().append("\n").append(getCopyRights());
}
