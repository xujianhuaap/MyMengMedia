package cn.skullmind.mbp.transcode

import java.lang.RuntimeException

enum class Filter( val label:String,
                   val supportType:SupportType,
                   val desc:String,
                   val argCount:Int,
                   val cmd:String
){
        //音频分割${outputs[1]}
        AcrossOver("音频分割",SupportType.AUDIO,"音频频段分割",4,
            "ffmpeg -y -i %0s -filter_complex acrossover=split=%1s[LOW][HIGH] -map [LOW] %2s -map [HIGH] %3s"),
}

enum class SupportType{
    AUDIO,
    VIDEO,
    AUDIO_VIDEO,
}

class FilterCmd(val filter: Filter):Cmd{

   override fun getCmd(args:Array<String>):String {
       if(args.size != filter.argCount) throw RuntimeException(" argument count is not correct")
       var cmd = filter.cmd
       args.forEachIndexed { index, s ->
           val placeHolder ="%${index}s"
           cmd = cmd.replace(placeHolder,s)
       }


        return  cmd
    }
}

interface Cmd{
    fun getCmd(args:Array<String>):String
}