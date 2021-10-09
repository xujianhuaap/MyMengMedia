//
// Created by xu on 21-9-30.
//

#include "BaseDecoder.h"

void BaseDecoder::Start() {
    if(m_thread == nullptr){
        startDecodingThread();
    } else{
        std::unique_lock<std::mutex>lock(m_mutex);
        m_decoder_state = STATE_DECODING;
        m_cond.notify_all();

    }
}

void BaseDecoder::Pause() {
    std::unique_lock<std::mutex>lock(m_mutex);
    m_decoder_state = STATE_PAUSE;
}

void BaseDecoder::Stop() {
    Log::d("BaseDecoder stop");
    std::unique_lock<std::mutex> lock(m_mutex);
    m_decoder_state = STATE_STOP;
    m_cond.notify_all();

}

void BaseDecoder::SeekToPosition(float position) {
    Log::d("BaseDecoder seekToPosition %f",position);
    std::unique_lock<std::mutex> lock(m_mutex);
    m_seek_postion = position;
    m_decoder_state = STATE_DECODING;
    m_cond.notify_all();
}

float BaseDecoder::GetCurrentPosition() {
    return m_current_timestamp;
}

int BaseDecoder::init(const char *url, AVMediaType mediaType) {
    Log::d("BaseDecoder init url -> %s, media type -> %d",url,mediaType);
    strcpy(m_url,url);
    m_media_type = mediaType;
    return 0;
}

void BaseDecoder::unInit() {
    if(m_thread){
        Stop();
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }
    Log::d("BasDecoder unInit");
}

int BaseDecoder::initDecoder() {
    Log::d("BaseDecoder init decoder");
    int result = 0;

    do{
        m_format_context = avformat_alloc_context();
        result = avformat_open_input(&m_format_context,m_url,NULL,NULL);
        if( result < 0){
            Log::d("BaseDecoder initDecoder open input file failure");
           break;
        }

        result = avformat_find_stream_info(m_format_context,NULL);
        if(result < 0){
            Log::d("BaseDecoder initDecoder find stream failure");
           break;
        }

        for(auto index = 0; index < m_format_context->nb_streams; index++){
            if(m_format_context->streams[index]->codecpar->codec_type == m_media_type){
                m_stream_index = index;
                break;
            }
        }

        if(m_stream_index == INVALID_STREAM_INDEX){
            result = -1;
            Log::d("BaseDecoder init decoder has no special media type stream");
            break;
        }

        AVCodecParameters* avCodecParameters = m_format_context->streams[m_stream_index]
                ->codecpar;
        m_av_codec = avcodec_find_decoder(avCodecParameters->codec_id);
        if(m_av_codec == nullptr){
            result = -1;
            Log::d("BaseDecoder init decoder no find decoder");
            break;
        }

        m_av_codec_context = avcodec_alloc_context3(m_av_codec);
        result = avcodec_parameters_to_context(m_av_codec_context,avCodecParameters);
        if(result < 0){
            Log::d("BaseDecoder init decoder init av_codec_context failure");
           break;
        }

        AVDictionary * avDictionary = nullptr;
        av_dict_set(&avDictionary,"buffer_size","1024000",0);
        av_dict_set(&avDictionary,"stimeout","20000000",0);
        av_dict_set(&avDictionary,"max_delay","30000000",0);
        av_dict_set(&avDictionary,"rtsp_transport","tcp",0);

        result = avcodec_open2(m_av_codec_context,m_av_codec,&avDictionary);
        if(result < 0){
            Log::d("BaseDecoder init decoder codec open failure");
           break;
        }



        m_duration = m_format_context->duration/AV_TIME_BASE *1000;
        m_packet_data = av_packet_alloc();
        m_av_frame = av_frame_alloc();
    }while (false);



    if(result < 0 && m_message_call_back && m_msg_context){
        m_message_call_back(m_msg_context,MSG_DECODER_INIT_ERR,0);
    }

    return result;
}


void BaseDecoder::unInitDecoder() {
    Log::d("BaseDecoder unInit decoder");
    if(m_av_frame){
        av_frame_free(&m_av_frame);
        m_av_frame = nullptr;
    }

    if(m_packet_data){
        av_packet_free(&m_packet_data);
        m_packet_data = nullptr;
    }


    if(m_av_codec_context){
        avcodec_close(m_av_codec_context);
        avcodec_free_context(&m_av_codec_context);
        m_av_codec_context = nullptr;
        m_av_codec = nullptr;
    }


    if(m_format_context){
        avformat_close_input(&m_format_context);
        avformat_free_context(m_format_context);
        m_format_context = nullptr;
    }

}


void BaseDecoder::startDecodingThread() {
    m_thread = new std::thread(doAvDecoding,this);
}


//单位是毫秒
 long long BaseDecoder::GetSysCurrentTime()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    long long curTime = time.tv_usec/1000;
    return curTime;
}

void BaseDecoder::decodingLoop() {
    Log::d("BaseCoder decoding loop %d",m_media_type);
    {
        std::unique_lock<std::mutex>lock(m_mutex);
        m_decoder_state = STATE_DECODING;
        lock.unlock();
    }


    for(;;){
        while (m_decoder_state == STATE_PAUSE){
            std::unique_lock<std::mutex>lock(m_mutex);
            m_cond.wait_for(lock,std::chrono::milliseconds(10));
            m_start_timestamp = GetSysCurrentTime() - m_current_timestamp;
        }

        if(m_decoder_state == STATE_STOP){
            break;
        }

        if(m_start_timestamp == -1){
            m_start_timestamp = GetSysCurrentTime();
        }

        if(decodePacketData() != 0){
            std::unique_lock<std::mutex>lock(m_mutex);
            m_decoder_state = STATE_PAUSE;

        }
    }

    Log::d("BaseDecoder decode loop end");

}


void BaseDecoder::updateTimeStamp() {
    Log::d("BaseDecoder updateTimeStamp");
    std::unique_lock<std::mutex>lock(m_mutex);
    if(m_av_frame->pkt_dts != AV_NOPTS_VALUE){
        m_current_timestamp = m_av_frame->pkt_dts;
    } else if(m_av_frame->pts != AV_NOPTS_VALUE){
        m_current_timestamp = m_av_frame->pts;
    } else{
        m_current_timestamp = 0;
    }

    m_current_timestamp = (int64_t)((m_current_timestamp *
            av_q2d(m_format_context->streams[m_stream_index]->time_base))*1000);

    if(m_seek_postion > 0 && m_seek_success){
        m_start_timestamp = GetSysCurrentTime() - m_current_timestamp;
        m_seek_postion = 0;
        m_seek_success = false;

    }

}


long BaseDecoder::avSync() {
    Log::d("BaseDecoder av sync");
    long currentTime = GetSysCurrentTime();
    long elapsedTime = currentTime - m_start_timestamp;
    if(m_current_timestamp > elapsedTime ){
        auto sleepTime = static_cast<unsigned int>(m_current_timestamp - elapsedTime);
        sleepTime = sleepTime > DELAY_THRESHOLD?DELAY_THRESHOLD:sleepTime;
        av_usleep(sleepTime*1000);//微秒
    }

    return elapsedTime - m_current_timestamp;
}




int BaseDecoder::decodePacketData() {
    if(m_seek_postion > 0){
        int64_t seek_target = static_cast<int64_t>(m_seek_postion * 1000000);
        int64_t seek_min = INT64_MIN;
        int64_t  seek_max = INT64_MAX;
        int seek_result = avformat_seek_file(m_format_context,
                -1,seek_min,seek_target,seek_max,0);
        if(seek_result < 0){
            m_seek_success = false;
        } else{
            m_seek_success = true;
            if(m_stream_index != INVALID_STREAM_INDEX){
                avcodec_flush_buffers(m_av_codec_context);
            }
            ClearCache();
        }
    }

    //读取流中的下一个frame
    int result = av_read_frame(m_format_context,m_packet_data);
    while (result == 0){
        if(m_packet_data->stream_index == m_stream_index){
            //发送原始packet data 给 decoder
            if(avcodec_send_packet(m_av_codec_context,m_packet_data) == AVERROR_EOF){
                //解码结束
                result = -1;
                goto __EXIT;
            }

            int frame_count = 0;
            while (avcodec_receive_frame(m_av_codec_context,m_av_frame) == 0){
                updateTimeStamp();
                avSync();
                onFrameAvailable(m_av_frame);
                frame_count++;
            }

            if(frame_count > 0){
                result = 0;
                goto __EXIT;
            }


        }

        av_packet_unref(m_packet_data);
        result = av_read_frame(m_format_context,m_packet_data);

    }

    __EXIT:
    av_packet_unref(m_packet_data);
    return result;
}

void BaseDecoder::doAvDecoding(BaseDecoder *baseDecoder) {
    if(baseDecoder->initDecoder() != 0) return;
    baseDecoder->onDecodeReady();
    baseDecoder->decodingLoop();
    baseDecoder->unInitDecoder();
    baseDecoder->onDecodeDone();
}