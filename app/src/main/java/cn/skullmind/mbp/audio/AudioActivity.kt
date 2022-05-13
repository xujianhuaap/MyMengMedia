package cn.skullmind.mbp.audio

import android.Manifest
import android.app.AlertDialog
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import androidx.core.app.ActivityCompat
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.FragmentActivity
import cn.skullmind.mbp.R
import cn.skullmind.mbp.databinding.ActivityAudioBinding
import cn.skullmind.mbp.media.MediaPlayer
import cn.skullmind.mbp.media.MediaRecorderContext
import cn.skullmind.mbp.media.MediaRecorderStatusListener
import cn.skullmind.mbp.media.PRO_TAG
import cn.skullmind.mbp.utils.getAudioPath

fun startAudioActivity(context: Context) {
    context.startActivity(Intent(context, AudioActivity::class.java))
}

class AudioActivity : FragmentActivity() {


    private lateinit var binding: ActivityAudioBinding
    lateinit var audioJob: AudioJob
    lateinit var mediaRecorder: MediaRecorderContext
    lateinit var meidiaPlayer: MediaPlayer
    private val callBack = object : RecordStatusListener {
        override fun onError(msg: String) {
            Log.d(PRO_TAG, msg)

        }

        override fun onAudioData(data: ByteArray, len: Int) {
            mediaRecorder.onAudioData(data, len)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        initData()

    }

    private lateinit var alertDialog: AlertDialog

    private fun initData() {
        binding = DataBindingUtil.setContentView(this, R.layout.activity_audio)
        binding.also {
            it.tvStartRecord.setOnClickListener { clkStartRecord() }
            it.tvStopRecord.setOnClickListener { clkStopRecord() }
            it.tvPlayerRecord.setOnClickListener { clkPlayRecord() }
        }

        audioJob = AudioJob(callBack)
        mediaRecorder = MediaRecorderContext()
        mediaRecorder.statusListener = object : MediaRecorderStatusListener {
            override fun onAudioStatus(status: Int) {
                when (status) {
                    MediaRecorderContext.STATUS_START -> {
                        if (!this@AudioActivity::alertDialog.isInitialized) {
                            alertDialog = AlertDialog.Builder(this@AudioActivity)
                                .setMessage("...recording...")
                                .setCancelable(false)
                                .setNegativeButton("停止录音"){ _,_ ->

                                    clkStopRecord()
                                }
                                .create()
                        }
                        alertDialog.show()
                    }

                    MediaRecorderContext.STATUS_END ->
                        alertDialog.dismiss()
                }
            }
        }

        meidiaPlayer = MediaPlayer("/storage/emulated/0/Android/data/cn.skullmind.mbp/files/Music/mymeng_audio_1652434153314.aac")
    }


    private fun clkStartRecord() {
        if (!hasPermissionsGranted()) {
            requestPermissions(REQUEST_PERMISSIONS, PERMISSION_REQUEST_CODE)
            return
        }
        getAudioPath(this, "aac")?.apply {
            audioJob.start()
            mediaRecorder.createContext()
            mediaRecorder.startRecordAudio(this.absolutePath)
        }

    }

    private fun clkStopRecord() {
        audioJob.cancel()
        mediaRecorder.stopRecord()
        mediaRecorder.destroyContext()
    }

    private fun clkPlayRecord() {
       meidiaPlayer.play()
    }

    private fun hasPermissionsGranted(): Boolean {
        for (permission in REQUEST_PERMISSIONS) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                != PackageManager.PERMISSION_GRANTED
            ) {
                return false
            }
        }
        return true
    }

    companion object {
        private val REQUEST_PERMISSIONS = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO
        )
        private const val PERMISSION_REQUEST_CODE = 0x12
    }
}