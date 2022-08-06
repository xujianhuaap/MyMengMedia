package cn.skullmind.mbp.media

import android.util.Log
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch

class MediaPlayer(val status: MediaPlayerStatus,val mediaType:Int) {

   private var nativePlayerHandle = 0L
    constructor(url: String,status: MediaPlayerStatus,mediaType:Int) : this(status,mediaType) {
        nativeInit(url)
    }

    fun play(){
        nativePlay()
    }

    fun stop(){
        nativeStop()
        nativeUnInit()
    }

    fun playerEventCallback(msgType:Int,msgCode:Float){
        MainScope().launch {
            if(msgType == MsgType.MSG_DECODING_TIME.value){
                Log.d("-->","$msgCode")
                status.currentPos(msgCode)
            }

        }

    }
    private external fun nativePlay()
    private external fun nativePause()
    private external fun nativeStop()
    private external fun nativeSeekToPos(pos:Int)
    private external fun nativeInit(url:String)
    private external fun nativeUnInit()


    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}

enum class MediaType(val value: Int){
    AVMEDIA_TYPE_UNKNOWN(-1),  ///< Usually treated as AVMEDIA_TYPE_DATA
    AVMEDIA_TYPE_VIDEO(0),
    AVMEDIA_TYPE_AUDIO(1),
    AVMEDIA_TYPE_DATA(2),          ///< Opaque data information usually continuous
    AVMEDIA_TYPE_SUBTITLE(3),
    AVMEDIA_TYPE_ATTACHMENT(4),    ///< Opaque data information usually sparse
    AVMEDIA_TYPE_NB(4)
}

enum class MsgType(val value:Int){
    MSG_DECODER_INIT_ERR(0),
    MSG_DECODER_READY(1),
    MSG_DECODER_DONE(2),
    MSG_REQUEST_RENDER(3),
    MSG_DECODING_TIME(4)
}

interface MediaPlayerStatus{
    fun currentPos(progress:Float)
}