package cn.skullmind.mbp.media

import android.util.Log
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch

const val PRO_TAG = "MyMengMedia"
open class MediaRecorderContext {
    val RECORDER_TYPE_SINGLE_AUDIO = 1 //仅录制音频
    var statusListener:MediaRecorderStatusListener? = null
    external fun createContext()
    external fun destroyContext()
    external fun init()
    external fun unInit()
    external fun startRecordAudio(outputUrl:String):Int
    external fun onAudioData(data:ByteArray,len:Int)
    external fun stopRecord()
    var nativeContextHandle = 0L

    fun onAudioStatus(status:Int,msg:String){
        MainScope().launch {
            Log.d("--->","$msg")
            statusListener?.onAudioStatus(status)
        }
    }
    companion object{
        @JvmStatic
        val STATUS_START = 0
        @JvmStatic
        val STATUS_Err = 2
        @JvmStatic
        val STATUS_END = 1
        init {
            System.loadLibrary("native-lib")
        }
    }
}

interface  MediaRecorderStatusListener {
    fun onAudioStatus(status:Int)
}