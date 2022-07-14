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

std::string getLicense(){
    std::string str =     "FFMpeg is free software; you can redistribute it and/or\n"
                          "modify it under the terms of the GNU Lesser General Public\n"
                          "License as published by the Free Software Foundation; either\n"
                          "version 2.1 of the License, or (at your option) any later version.\n"
                          "\n"
                          "FFMpeg is distributed in the hope that it will be useful,\n"
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
                          "Lesser General Public License for more details.\n"
                          "\n"
                          "You should have received a copy of the GNU Lesser General Public\n"
                          "License along with FFMpeg; if not, write to the Free Software\n"
                          "Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA\n";
    return str;
}

std::string getFFMpegInfo(){
    return getVersion().append("\n").append(getCopyRights())
    .append("\n\n").append(getLicense());
}
