package cn.skullmind.mbp

import android.content.Context
import android.os.Bundle
import android.view.*
import android.widget.BaseAdapter
import android.widget.GridView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import cn.skullmind.mbp.audio.startAudioActivity
import cn.skullmind.mbp.transcode.startTransCodeActivity
import com.google.android.material.floatingactionbutton.FloatingActionButton
import java.util.*

class MainActivity : AppCompatActivity() {
    private val funcList = listOf(
        FuncItem("音频录制",this::jumpToRecordAudio),
        FuncItem("转码",this::jumpToTranscode)
    )

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(findViewById(R.id.toolbar))
        findViewById<GridView>(R.id.grv_functions).run {
            this.adapter = FuncAdapter(this@MainActivity, funcList)

        }


    }

    private fun jumpToRecordAudio(){
        startAudioActivity(this)
    }

    private fun jumpToTranscode(){
        startTransCodeActivity(this)
    }
    private fun temp() {
        findViewById<FloatingActionButton>(R.id.grv_functions).setOnClickListener { view ->
            //            Toast.makeText(this, OSInfo().getOSString(), Toast.LENGTH_LONG).show()
            //            val coder = AudioCoder()
            //            val recorder = VideoCapture()
            //            val file = File(getExternalFilesDir(DIRECTORY_MUSIC),"test.mp3")
            //            if(file.exists()) file.delete()
            //            file.createNewFile()
            //            resources.assets.open("test.mp3").copyTo(FileOutputStream(file))
            //
            //
            //            val pcmFile = File(file.parent,coder.getPCMFileName(file.name))
            //            recorder.captureCameraData(pcmFile)
            //            val audioManager = getSystemService(AudioManager::class.java) as AudioManager
            //            val player = AudioTrackPlayer(audioManager.generateAudioSessionId(),pcmFile)
            //            GlobalScope.launch {
            //                if(pcmFile.exists()){
            //                    pcmFile.delete()
            //                }
            //                pcmFile.createNewFile()
            //                coder.generatePCMFile(file)
            //                if(pcmFile.canRead()){
            //                    player.play()
            //                }
            //            }



        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_settings -> true
            else -> super.onOptionsItemSelected(item)
        }
    }


}

class FuncAdapter(private val context: Context, private val items: List<FuncItem>) : BaseAdapter() {
    override fun getCount() = items.size

    override fun getItem(position: Int) = items[position]

    override fun getItemId(position: Int) = 0L

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val rootView: View = Optional.ofNullable(convertView)
            .orElse(LayoutInflater.from(context).inflate(R.layout.item_ffmpeg_func, parent, false))

       return rootView.apply {
            this.setOnClickListener { items[position].callBack() }
            this.findViewById<TextView>(R.id.tv_func).text = items[position].name
        }
    }

}

data class FuncItem(val name: String, val callBack:()->Unit)