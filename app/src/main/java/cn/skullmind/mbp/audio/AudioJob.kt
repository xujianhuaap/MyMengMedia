package cn.skullmind.mbp.audio

import android.Manifest
import android.content.pm.PackageManager
import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import android.util.Log
import androidx.core.app.ActivityCompat
import cn.skullmind.mbp.media.PRO_TAG
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.launch

class AudioJob(private val listener: RecordStatusListener) {
    lateinit var audioRecord: AudioRecord
    val channel = Channel<Int>()

    fun start() {
        GlobalScope.launch {
            val minBufferSize = AudioRecord.getMinBufferSize(
                DEFAULT_SAMPLE_RATE, DEFAULT_CHANNEL_LAYOUT,
                DEFAULT_SAMPLE_FORMAT
            )
            if (AudioRecord.ERROR_BAD_VALUE == minBufferSize) {
                Log.d(PRO_TAG, "hardware do not support config")
                listener.onError("hardware do not support config")
                return@launch
            }

            audioRecord = AudioRecord(
                MediaRecorder.AudioSource.MIC, DEFAULT_SAMPLE_RATE,
                DEFAULT_CHANNEL_LAYOUT, DEFAULT_SAMPLE_FORMAT, minBufferSize
            )
            try {
                audioRecord.startRecording()
            } catch (e: RuntimeException) {
                e.printStackTrace()
                e.message?.let { listener.onError(it) }
                return@launch
            }
            val buffer = ByteArray(4096)
            try {
                while (!Thread.currentThread().isInterrupted
                    || channel.receive() == EOF_FLAG) {
                    val result = audioRecord.read(buffer, 0, 4096)
                    if (result > 0) {
                        listener.onAudioData(buffer, result)
                    }
                }

            } catch (e: RuntimeException) {
                e.printStackTrace()
                e.message?.let {
                    listener.onError(it)
                }
            }

            audioRecord.release()
            channel.close()
        }
    }

    fun cancel(){
        GlobalScope.launch {
            channel.send(EOF_FLAG)
        }
    }


    companion object {
        private const val DEFAULT_SAMPLE_RATE = 44100
        private const val DEFAULT_CHANNEL_LAYOUT = AudioFormat.CHANNEL_IN_STEREO
        private const val DEFAULT_SAMPLE_FORMAT = AudioFormat.ENCODING_PCM_16BIT
        private const val EOF_FLAG  = 0x00
    }
}

interface RecordStatusListener {
    fun onError(msg: String)
    fun onAudioData(data: ByteArray, len: Int)
}
