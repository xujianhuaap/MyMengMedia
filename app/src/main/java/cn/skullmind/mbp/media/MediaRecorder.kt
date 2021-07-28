package cn.skullmind.mbp.media

import java.io.File

class VideoCapture {
    external fun captureCameraData(outputFile:File):Int

    companion object {
        init {

            System.loadLibrary("native-lib")
        }
    }
}

class MediaRecorder {
    var nativeContextHandle = 0L
}