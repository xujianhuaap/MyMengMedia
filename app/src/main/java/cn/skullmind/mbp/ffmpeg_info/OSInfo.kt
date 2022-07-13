package cn.skullmind.mbp.ffmpeg_info

class OSInfo {
    external fun getOSString():String
    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}