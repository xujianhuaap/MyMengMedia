package cn.skullmind.mbp.transcode

object FFMpeg {

    fun transcode(cmd:String){
        val arr = cmd.split(" ").toTypedArray()
        nativeCmd(args = arr)
    }

    fun clear(){
        nativeCleanUp()
    }


    private external fun nativeCmd(args:Array<String>):Int
    private external fun nativeCleanUp()

    init {
        System.loadLibrary("native-lib")
    }
}