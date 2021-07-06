package cn.skullmind.mbp

import android.os.Bundle
import android.os.Environment.DIRECTORY_MUSIC
import android.view.Menu
import android.view.MenuItem
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import cn.skullmind.mbp.media.AudioCoder
import com.google.android.material.floatingactionbutton.FloatingActionButton
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import java.io.File
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(findViewById(R.id.toolbar))
        findViewById<FloatingActionButton>(R.id.fab).setOnClickListener { view ->
            Toast.makeText(this, OSInfo().getOSString(), Toast.LENGTH_LONG).show()
            val coder = AudioCoder()

            val file = File(getExternalFilesDir(DIRECTORY_MUSIC),"test.mp3")
            if(file.exists()) file.delete()
            file.createNewFile()
            resources.assets.open("test.mp3").copyTo(FileOutputStream(file))
            coder.generatePCMFile(file)

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