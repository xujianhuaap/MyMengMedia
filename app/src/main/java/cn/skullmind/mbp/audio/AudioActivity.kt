package cn.skullmind.mbp.audio

import android.Manifest
import android.app.AlertDialog
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.TextView
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.FragmentActivity
import cn.skullmind.mbp.R
import cn.skullmind.mbp.databinding.ActivityAudioBinding
import cn.skullmind.mbp.media.*
import cn.skullmind.mbp.utils.getAudioPath
import cn.skullmind.mbp.utils.getRecordAudioFiles
import java.io.File

fun startAudioActivity(context: Context) {
    context.startActivity(Intent(context, AudioActivity::class.java))
}

class AudioActivity : FragmentActivity() {


    private lateinit var binding: ActivityAudioBinding
    lateinit var audioJob: AudioJob
    lateinit var mediaRecorder: MediaRecorderContext
    lateinit var mediaPlayer: MediaPlayer
    private var currentRecordFile:File? = null
    private val callBack = object : RecordStatusListener {
        override fun onError(msg: String) {
            Log.d(PRO_TAG, msg)

        }

        override fun onAudioData(data: ByteArray, len: Int) {
            mediaRecorder.onAudioData(data, len)
        }
    }

    private val playStatusListener = object : MediaPlayerStatus {
        override fun currentPos(progress: Float) {
            binding.playProgress.progress = (progress * 100).toInt()
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
                                .setNegativeButton("????????????") { _, _ ->

                                    clkStopRecord()
                                }
                                .create()
                        }
                        alertDialog.show()
                    }

                    MediaRecorderContext.STATUS_END ->
                        alertDialog.dismiss()
                    MediaRecorderContext.STATUS_Err ->{
                        currentRecordFile?.run{
                            if(this.exists()) delete()
                        }
                        alertDialog.dismiss()
                    }


                }
            }
        }


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
        val recordAudioFiles = getRecordAudioFiles(this)
        if(recordAudioFiles.isNotEmpty()){
            val recordAudioAdapter = RecordAudioAdapter(recordAudioFiles)
            AlertDialog.Builder(this).setCancelable(false)
                .setAdapter(recordAudioAdapter) { dialog, pos ->
                    if (this::mediaPlayer.isInitialized) mediaPlayer.stop()

                    val file = getRecordAudioFiles(this)[pos]
                    currentRecordFile = file
                    mediaPlayer = MediaPlayer(
                        file.absolutePath,
                        status = playStatusListener
                    )
                    mediaPlayer.play()
                    dialog.dismiss()

                }.create().show()
        }else {
            Toast.makeText(this,"??????????????????",Toast.LENGTH_LONG).show()
        }

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

class RecordAudioAdapter(val names: List<File>) : BaseAdapter() {
    override fun getCount(): Int = names.size
    override fun getItem(position: Int): Any = names[position]
    override fun getItemId(position: Int) = 0L
    override fun getView(position: Int, convertView: View?, parent: ViewGroup): View {
        return convertView ?: TextView(parent.context).apply {
            text = names[position].name
            val padding = resources.getDimensionPixelOffset(R.dimen.dimen_10dp)
            setPadding(0, padding, 0, padding)
        }
    }
}