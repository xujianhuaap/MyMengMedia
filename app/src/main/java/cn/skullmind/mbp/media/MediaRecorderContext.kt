package cn.skullmind.mbp.media

const val PRO_TAG = "MyMengMedia"
open class MediaRecorderContext {
    val RECORDER_TYPE_SINGLE_AUDIO = 1 //仅录制音频
    external fun createContext()
    external fun destroyContext()
    external fun init()
    external fun unInit()
    external fun startRecordAudio(outputUrl:String):Int
    external fun onAudioData(data:ByteArray,len:Int)
    external fun stopRecord()
    var nativeContextHandle = 0L

    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}