package cn.skullmind.mbp.transcode

import android.Manifest
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import cn.skullmind.mbp.R
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

fun startTransCodeActivity(context: Context) {
    context.startActivity(Intent(context, TransCodeActivity::class.java))
}
class TransCodeActivity:AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_trans_code)

    }

    override fun onResume() {
        super.onResume()
        checkPermissions()

    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if(requestCode == CODE_PERMISSION_STORAGE){
            if(grantResults.all { it ==   PackageManager.PERMISSION_GRANTED}){
                startTranscode()
            }
        }
    }

    private fun checkPermissions() {
        if (permissions.all { checkSelfPermission(it) == PackageManager.PERMISSION_GRANTED }) {
            startTranscode()
        }else {
            requestPermissions(permissions, CODE_PERMISSION_STORAGE)
        }
    }

    private fun startTranscode() {
        GlobalScope.launch {
            FFMpeg().transcode("ffmpeg -y -i /storage/emulated/0/mymeng_audio_1657677635382.aac -filter_complex acrossover=split=1500[LOW][HIGH] -map [LOW] /storage/emulated/0/low.wav -map [HIGH] /storage/emulated/0/high.wav")

        }
    }



    companion object {
        private val permissions = arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE)
        private const val CODE_PERMISSION_STORAGE = 1001
    }
}