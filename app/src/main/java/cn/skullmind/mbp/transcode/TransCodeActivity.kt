package cn.skullmind.mbp.transcode

import android.Manifest
import android.annotation.SuppressLint
import android.app.AlertDialog
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import cn.skullmind.mbp.R
import cn.skullmind.mbp.audio.RecordAudioAdapter
import cn.skullmind.mbp.utils.getFileName
import cn.skullmind.mbp.utils.getRecordAudioFiles
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import java.io.File
import java.util.*

fun startTransCodeActivity(context: Context) {
    context.startActivity(Intent(context, TransCodeActivity::class.java))
}

class TransCodeActivity : AppCompatActivity() {
    private lateinit var tvFilterStatus:TextView
    private lateinit var tvFilterInput:TextView
    private lateinit var recFilterTranscodeResult:RecyclerView
    private val transCodeAdapter = TransCodeAdapter()

    private lateinit var vm:TranscodeViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_trans_code)

        initView()
        initVm()
        selectTranscodeFile()
    }

    private fun initVm() {
        vm = ViewModelProvider(this)[TranscodeViewModel::class.java]
        vm.getSelectFile().observe(this){
            it?.run {
                tvFilterInput.text = this.name
            }
        }

        vm.getTranscodeStatus().observe(this){status ->
            if(status == 0){
                tvFilterStatus.text = "转码成功"
            }else {
                tvFilterStatus.text = "转码失败"
            }

        }

        vm.getTranscodeResultFiles().observe(this){
            transCodeAdapter.setData(Optional.ofNullable(it).orElse(ArrayList()))
        }

    }



    private fun initView() {
        tvFilterStatus = findViewById(R.id.tv_filter_status)
        tvFilterStatus.setOnClickListener {
            vm.refreshTranscodeFiles()
        }
        tvFilterInput = findViewById(R.id.tv_filter_input)
        recFilterTranscodeResult = findViewById(R.id.rec_transcode_files)
        recFilterTranscodeResult.setHasFixedSize(true)
        recFilterTranscodeResult.layoutManager = LinearLayoutManager(this)
        recFilterTranscodeResult.adapter = transCodeAdapter
        val dividerItemDecoration = DividerItemDecoration(this, LinearLayout.VERTICAL).apply {
            setDrawable(getDrawable(R.drawable.shape_divider_rect)!!)
        }
        recFilterTranscodeResult.addItemDecoration(dividerItemDecoration)
    }

    private fun selectTranscodeFile() {
        val recordAudioFiles = getRecordAudioFiles(this)
        if (recordAudioFiles.isNotEmpty()) {
            val recordAudioAdapter = RecordAudioAdapter(recordAudioFiles)
            AlertDialog.Builder(this).setCancelable(false)
                .setAdapter(recordAudioAdapter) { dialog, pos ->
                    vm.refreshSelectFile(recordAudioFiles[pos])
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
                vm.startTranscode()
            }
        }
    }

    private fun checkPermissions() {
        if (permissions.all { checkSelfPermission(it) == PackageManager.PERMISSION_GRANTED }) {
            vm.startTranscode()
        } else {
            requestPermissions(permissions, CODE_PERMISSION_STORAGE)
        }
    }



    override fun onDestroy() {
        FFMpeg.clear()
        super.onDestroy()
    }

    companion object {
        private val permissions = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE
        )
        private const val CODE_PERMISSION_STORAGE = 1001
    }
}



class TranscodeViewModel:ViewModel(){
    private val selectFile  = MutableLiveData<File?>()
    private val transcodeFiles = MutableLiveData<List<File>?>()
    private val transcodeStatus = MutableLiveData<Int>()

    fun getSelectFile() = selectFile
    fun refreshSelectFile(file:File?){
        selectFile.value = file
    }
    fun getTranscodeResultFiles() = transcodeFiles

    fun getTranscodeStatus() = transcodeStatus

     fun startTranscode() {
        GlobalScope.launch {
            selectFile.value?.run {
                transcodeStatus.postValue(FFMpeg.transcode(getCmdStr(this)))
                if(transcodeStatus.value == 0){
                    transcodeFiles.postValue(getTranscodeResult())
                }
            }
        }
    }

    private fun getCmdStr(inputFile: File): String {
        val filterDir = getTranscodeResultDir(inputFile)
        filterDir.listFiles()?.forEach { it.delete() }
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


    fun refreshTranscodeFiles(){
        transcodeFiles.postValue(getTranscodeResult())
    }
    private fun getTranscodeResultDir(inputFile: File): File {
        val dirStr: String = inputFile.parentFile!!.absolutePath.plus("/Filter")
        val filterDir = File(dirStr)
        if(!filterDir.exists()) filterDir.mkdir()
        return filterDir
    }

     private fun getTranscodeResult():List<File>?{
        return selectFile.value?.run {
            getTranscodeResultDir(this).listFiles()?.filter { it.isFile }
        }
    }

}

class TransCodeAdapter:RecyclerView.Adapter<TransCodeViewHolder>(){
    private val files = ArrayList<File>()
    @SuppressLint("NotifyDataSetChanged")
    fun setData(files:List<File>){
        this.files.clear()
        this.files.addAll(files)
        notifyDataSetChanged()
    }
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): TransCodeViewHolder {
        val rootView = LayoutInflater.from(parent.context).inflate(R.layout.item_ffmpeg_transcode_file,parent,false)
        return TransCodeViewHolder(rootView)
    }

    override fun onBindViewHolder(holder: TransCodeViewHolder, position: Int) {
        holder.bind(files[position])
    }

    override fun getItemCount(): Int  = files.size
}

class TransCodeViewHolder(rootView: View):RecyclerView.ViewHolder(rootView){
    fun bind(file:File){
        itemView.findViewById<TextView>(R.id.tv_transcode_file).text = file.name
    }
}