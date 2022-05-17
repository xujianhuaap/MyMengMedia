package cn.skullmind.mbp.media

import android.util.Log

class MediaPlayer {

   private var nativePlayerHandle = 0L

    constructor(url: String){
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
        if(msgType == MsgType.MSG_DECODING_TIME.value){
            Log.d("-->","$msgCode")
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

enum class MsgType(val value:Int){
    MSG_DECODER_INIT_ERR(0),
    MSG_DECODER_READY(1),
    MSG_DECODER_DONE(2),
    MSG_REQUEST_RENDER(3),
    MSG_DECODING_TIME(4)

}