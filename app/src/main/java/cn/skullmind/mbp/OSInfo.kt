package cn.skullmind.mbp

class OSInfo {
    external fun getOSString():String
    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}