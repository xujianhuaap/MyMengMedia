package cn.skullmind.mbp.media

import android.text.TextUtils
import java.io.File

class AudioCoder {
    external fun generatePCMFile(sourceFile: File):Int
    fun getPCMFileName(sourceFileName:String):String = sourceFileName.let {
        val values = it.split(".")
        values[0].plus("_pcm.").plus(values[1])
    }
    fun validSourceFileName(sourceFileName:String):Boolean {
        if(TextUtils.isEmpty(sourceFileName))return false
        if(!sourceFileName.contains("."))return false
        val vlaues = sourceFileName.split(".")
        if(vlaues.size != 2) return false
        return true
    }
    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}