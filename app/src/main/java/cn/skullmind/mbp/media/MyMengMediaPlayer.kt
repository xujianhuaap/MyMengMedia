package cn.skullmind.mbp.media

class MyMengMediaPlayer {
    var nativeContextHandle = 0L
    external fun getPlayerHandle():Long
    external fun play()

    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}