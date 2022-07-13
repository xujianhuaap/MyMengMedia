package cn.skullmind.mbp

import android.annotation.SuppressLint
import android.content.Context
import android.os.Bundle
import android.view.*
import android.widget.BaseAdapter
import android.widget.GridView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import cn.skullmind.mbp.audio.startAudioActivity
import cn.skullmind.mbp.ffmpeg_info.startFFmpegInfoActivity
import cn.skullmind.mbp.transcode.startTransCodeActivity
import java.util.*

class MainActivity : AppCompatActivity() {
    private val funcList = listOf(
        FuncItem("FF信息",this::jumpToFFMpegInfo),
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

    private fun jumpToFFMpegInfo() {
        startFFmpegInfoActivity(this)
    }


    private fun jumpToRecordAudio(){
        startAudioActivity(this)
    }

    private fun jumpToTranscode(){
        startTransCodeActivity(this)
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

    @SuppressLint("ViewHolder")
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