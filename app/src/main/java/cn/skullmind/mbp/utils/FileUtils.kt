package cn.skullmind.mbp.utils

import android.content.Context
import android.os.Environment
import java.io.File
import java.text.SimpleDateFormat
import java.util.*

fun getAudioPath(context: Context, ext: String): File? {
    return getParentDir(context)?.run {
        val fileName = getFileName("mymeng_audio_",ext)
        File(this.absolutePath, fileName).apply {
            if(this.exists()) this.delete()
            this.createNewFile()
        }

    }
}

 fun getFileName(name:String,ext: String): String {
     val time = SimpleDateFormat("yyyy_MM_dd_HH_mm_ss", Locale.CHINESE).format(Date())
     return name
        .plus(time)
        .plus(".")
        .plus(ext)
}

fun getRecordAudioFiles(context: Context):List<File> {
    val list = mutableListOf<File>()
    getParentDir(context)?.listFiles()?.forEach {
        if(it.isFile && it.canRead()) list.add(it)
    }
    return list
}

private fun getParentDir(context: Context):File?{
    return context.getExternalFilesDir(Environment.DIRECTORY_MUSIC)
}
