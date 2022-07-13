package cn.skullmind.mbp.ffmpeg_info

import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import cn.skullmind.mbp.R

fun startFFmpegInfoActivity(context: Context) {
    context.startActivity(Intent(context, FFmpegInfoActivity::class.java))
}
class FFmpegInfoActivity:AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_trans_code)

    }


}