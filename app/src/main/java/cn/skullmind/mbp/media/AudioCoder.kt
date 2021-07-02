package cn.skullmind.mbp.media

class AudioCoder {
    external fun readSamples():Int
    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}