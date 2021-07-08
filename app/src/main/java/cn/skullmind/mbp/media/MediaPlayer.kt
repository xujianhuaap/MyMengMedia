package cn.skullmind.mbp.media

import android.media.AudioAttributes
import android.media.AudioAttributes.*
import android.media.AudioFormat
import android.media.AudioFormat.*
import android.media.AudioManager
import android.media.AudioTrack
import java.io.File
import java.io.FileInputStream

class MediaPlayer:Player{
    override fun play() {
        TODO("Not yet implemented")
    }

    external fun supportMediaFormats():List<String>
    companion object{
        init {
            System.loadLibrary("native-lib")
        }
    }
}

/***
 * sessionId 由AudioManager产生.
 *
 * MediaTrack的transferMode 中的AudioTrack.MODE_STREAM 适用于播放长时间音乐场景,延迟较高,
 * AudioTrack.MODE_STATIC使用与占内存较小的,低延迟的场景,例如游戏声音
 *
 * setOffloadedPlayback()如果设置为true,transferMode必须是MODE_STREAM 并且AudioFormat必须在设备上
 * 支持才行(AudioManager.isOffloadedPlaybackSupported (AudioFormat format,AudioAttributes attributes)\
 * 该方法为true)
 *
 *
 * setPerformanceMode(),不是所有的设备支持,主要有三种, AudioTrack#PERFORMANCE_MODE_NONE,这种是默认的,
 * AudioTrack#PERFORMANCE_MODE_LOW_LATENCY这种意味者低延迟,低延迟要求缓存足够小.使用低延迟模式下在没有
 * 指定BufferSizeInBytes的情况下,会导致缓存非常小,创建AutoTrack实例的时候应该指定一个合适足够大的缓存,随后可以
 * setBufferSizeInFrames(int)改变缓存的大小;当延迟故障较高的时候可以使减小当前的缓存,延迟故障消失的时候可以适当
 * 放大缓存,寻求一个合适的折中点.
 * AudioTrack#PERFORMANCE_MODE_POWER_SAVING 这种是省电模式.通过增大缓存,可能会引起延迟较高的故障.
 */
class AudioTrackPlayer(private val sessionId: Int,val sourceFile:File) : Player {
    var bufferSize:Int = 0
    private val playerImp:AudioTrack = AudioTrack.Builder().let{
        val audioAttributes = AudioAttributes.Builder()
//            .setAllowedCapturePolicy(ALLOW_CAPTURE_BY_NONE)
            .setContentType(CONTENT_TYPE_UNKNOWN)
            .setUsage(USAGE_MEDIA)
//            .setFlags(FLAG_AUDIBILITY_ENFORCED or FLAG_HW_AV_SYNC)
//            .setHapticChannelsMuted(false)//是否触觉静音 默认是false
//            .setLegacyStreamType(AudioManager.STREAM_MUSIC)
            .build()

        val sampleRate = 4000 * 4
        val encodeFormat = ENCODING_PCM_16BIT
        val audioFormat = AudioFormat.Builder()
            .setSampleRate(sampleRate)
            .setEncoding(encodeFormat)
            .build()

        //CHANNEL_OUT_MONO 单声道
        //CHANNEL_OUT_STEREO 双声道

        bufferSize = AudioTrack.getMinBufferSize(sampleRate, CHANNEL_OUT_STEREO,encodeFormat)
        it.setAudioAttributes(audioAttributes)
            .setAudioFormat(audioFormat)
            .setBufferSizeInBytes(bufferSize)
            .setPerformanceMode(AudioTrack.PERFORMANCE_MODE_LOW_LATENCY)
            .setTransferMode(AudioTrack.MODE_STREAM)
            .setOffloadedPlayback(false)//是否支持已卸载的路径播放
            .setSessionId(sessionId)
            .build()
    }
    override fun play() {
        if(playerImp.state != AudioTrack.STATE_UNINITIALIZED) {
            playerImp.play()
            val fis = FileInputStream(sourceFile)
            val buffer = ByteArray(bufferSize)
            var len = 0
            while ((fis.read(buffer).also { len = it })!= -1){
                playerImp.write(buffer,0,len)
            }

        }
    }
}
interface Player{
    fun play()
}