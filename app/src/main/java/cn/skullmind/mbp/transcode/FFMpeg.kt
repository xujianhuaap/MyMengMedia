package cn.skullmind.mbp.transcode

class FFMpeg {
    fun transcode(cmd:String){
        val arr = cmd.split(" ").toTypedArray()
        nativeCmd(args = arr)
    }


    private external fun nativeCmd(args:Array<String>):Int

    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}