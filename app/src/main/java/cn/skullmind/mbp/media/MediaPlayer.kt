package cn.skullmind.mbp.media

class MediaPlayer {

   private var nativePlayerHandle = 0L

    constructor(url: String){
        nativeInit(url)
    }

    fun play(){
        nativePlay()
    }

    fun playerEventCallback(msgType:Int,msgCode:Float){

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