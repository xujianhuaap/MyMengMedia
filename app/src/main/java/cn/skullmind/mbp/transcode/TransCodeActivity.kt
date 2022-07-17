package cn.skullmind.mbp.transcode

import android.Manifest
import android.app.AlertDialog
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import cn.skullmind.mbp.R
import cn.skullmind.mbp.audio.RecordAudioAdapter
import cn.skullmind.mbp.utils.getFileName
import cn.skullmind.mbp.utils.getRecordAudioFiles
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import java.io.File

fun startTransCodeActivity(context: Context) {
    context.startActivity(Intent(context, TransCodeActivity::class.java))
}

class TransCodeActivity : AppCompatActivity() {
    private var selectFile: File? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_trans_code)
        selectTranscodeFile()

    }

    private fun selectTranscodeFile() {
        val recordAudioFiles = getRecordAudioFiles(this)
        if (recordAudioFiles.isNotEmpty()) {
            val recordAudioAdapter = RecordAudioAdapter(recordAudioFiles)
            AlertDialog.Builder(this).setCancelable(false)
                .setAdapter(recordAudioAdapter) { dialog, pos ->
                    selectFile = recordAudioFiles[pos]
                    checkPermissions()
                    dialog.dismiss()

                }.create().show()
        } else {
            Toast.makeText(this, "暂无播放列表", Toast.LENGTH_LONG).show()
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == CODE_PERMISSION_STORAGE) {
            if (grantResults.all { it == PackageManager.PERMISSION_GRANTED }) {
                startTranscode()
            }
        }
    }

    private fun checkPermissions() {
        if (permissions.all { checkSelfPermission(it) == PackageManager.PERMISSION_GRANTED }) {
            startTranscode()
        } else {
            requestPermissions(permissions, CODE_PERMISSION_STORAGE)
        }
    }

    private val ffMpeg = FFMpeg()

    private fun startTranscode() {
        GlobalScope.launch {
            selectFile?.run {

                ffMpeg.transcode(getCmdStr(this))
            }


        }
    }

    private fun getCmdStr(inputFile: File): String {
        val dirStr: String = inputFile.parentFile!!.absolutePath.plus("/Filter")
        val filterDir = File(dirStr)
        if (filterDir.exists()) {
            filterDir.deleteRecursively()
        }
        filterDir.mkdir()
        val lowOutPut = "${filterDir.absolutePath}/".plus(getFileName("low_", "wav"))
        val highOutput = "${filterDir.absolutePath}/".plus(getFileName("high_", "wav"))

        return FilterCmd(Filter.AcrossOver).getCmd(
            arrayOf(
                inputFile.absolutePath,
                "1500",
                lowOutPut,
                highOutput,

            )
        )
    }


    override fun onStop() {
        super.onStop()
        ffMpeg.clear()
    }

    companion object {
        private val permissions = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE
        )
        private const val CODE_PERMISSION_STORAGE = 1001
    }
}