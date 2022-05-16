package cn.skullmind.mbp.utils

import android.content.Context
import android.os.Environment.DIRECTORY_MUSIC
import java.io.File

fun getAudioPath(context: Context, ext: String): File? {
    return context.getExternalFilesDir(DIRECTORY_MUSIC)?.run {
        val fileName = "mymeng_audio_"
            .plus(System.currentTimeMillis())
            .plus(".")
            .plus(ext)
        File(this.absolutePath, fileName).apply {
            if(this.exists()) this.delete()
            this.createNewFile()
        }

    }
}

fun getRecordAudioFiles(context: Context):List<File> {
    val list = mutableListOf<File>()
    context.getExternalFilesDir(DIRECTORY_MUSIC)?.listFiles()?.forEach {
        if(it.isFile && it.canRead()) list.add(it)
    }
    return list
}