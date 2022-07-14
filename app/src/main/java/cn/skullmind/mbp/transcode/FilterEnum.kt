package cn.skullmind.mbp.transcode

import java.lang.RuntimeException

enum class Filter( val label:String,
                   val supportType:SupportType,
                   val desc:String,
                   val inputCount:Int,
                   val outputCount:Int,
                   val argCount:Int
){
        //音频分割
        AcrossOver("音频分割",SupportType.AUDIO,"音频频段分割",1,2,1),
}

enum class SupportType{
    AUDIO,
    VIDEO,
    AUDIO_VIDEO,
}

class FilterCmd(val filter: Filter):Cmd{

   override fun getCmd(inputs:Array<String>,outputs:Array<String>,args:Array<String>):String {
       if(inputs.size != filter.inputCount) throw RuntimeException("input  count is not correct")
       if(outputs.size != filter.outputCount) throw RuntimeException("output  count is not correct")
       if(args.size != filter.argCount) throw RuntimeException(" argument count is not correct")
        return  "ffmpeg -y -i ${inputs[0]} -filter_complex acrossover=split=${args[0]}[LOW][HIGH] -map [LOW] ${outputs[0]} -map [HIGH] ${outputs[1]}"
    }
}

interface Cmd{
    fun getCmd(inputs:Array<String>,outputs:Array<String>,args:Array<String>):String
}