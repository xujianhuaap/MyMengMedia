package cn.skullmind.mbp

import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import cn.skullmind.mbp.audio.startAudioActivity
import com.google.android.material.floatingactionbutton.FloatingActionButton

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(findViewById(R.id.toolbar))
        findViewById<FloatingActionButton>(R.id.fab).setOnClickListener { view ->
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

            startAudioActivity(this)

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