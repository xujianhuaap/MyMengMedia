package cn.skullmind.mbp.utils

import android.content.Context
import android.os.Environment.DIRECTORY_MUSIC
import android.os.Environment.getExternalStoragePublicDirectory
import java.io.File

fun getAudioPath(context: Context, ext: String): File? {
    return getParentDir(context)?.run {
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
    getParentDir(context)?.listFiles()?.forEach {
        if(it.isFile && it.canRead()) list.add(it)
    }
    return list
}

private fun getParentDir(context: Context):File {
    return getExternalStoragePublicDirectory(DIRECTORY_MUSIC)
}
