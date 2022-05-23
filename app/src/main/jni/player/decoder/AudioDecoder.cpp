//
// Created by xu on 21-9-30.
//

#include "AudioDecoder.h"
void AudioDecoder::onDecodeReady() {
    Log::d("AudioDecoder onDecoderReady");
    if(m_render){
        AVCodecContext*  codecCxt = getCodecContext();
        m_swrContext = swr_alloc();
        av_opt_set_int(m_swrContext,"in_channel_layout",codecCxt->channel_layout,0);
        av_opt_set_int(m_swrContext,"out_channel_layout",AUDIO_DST_CHANNEL_LAYOUT,0);
        av_opt_set_int(m_swrContext,"in_sample_rate",codecCxt->sample_rate,0);
        av_opt_set_int(m_swrContext,"out_sample_rate", AUDIO_DST_SAMPLE_RATE,0);
        av_opt_set_int(m_swrContext,"in_sample_fmt",codecCxt->sample_fmt,0);
        av_opt_set_int(m_swrContext,"out_sample_fmt",AUDIO_DST_SAMPLE_FMT,0);
        swr_init(m_swrContext);

        m_render->init();
    }else {
        Log::d("AudioDecoder audio render is null");
    }

}


void AudioDecoder::onFrameAvailable(AVFrame *avFrame) {
    Log::d("AudioDecoder onFrameAvailable pts %ld",avFrame->pkt_pts);
    if(m_render){
        m_nb_sample_per_channel = (int)av_rescale_rnd(avFrame->nb_samples,AUDIO_DST_SAMPLE_RATE,avFrame->sample_rate,AV_ROUND_UP);
        m_dst_frame_data_size = av_samples_get_buffer_size(avFrame->linesize,AUDIO_DST_CHANNEL_COUNTS,m_nb_sample_per_channel,AUDIO_DST_SAMPLE_FMT,1);
        m_audio_out_buffer = (uint8_t *)malloc(m_dst_frame_data_size);
        int result = swr_convert(m_swrContext,&m_audio_out_buffer,m_dst_frame_data_size/2,
                (const uint8_t**)avFrame->data,avFrame->nb_samples);

        if(result > 0){
            m_render->renderAudioFrame(m_audio_out_buffer,m_dst_frame_data_size);
        }
    }
}



void AudioDecoder::onDecodeDone() {
    Log::d("AudioDecoder onDecodeDone");
    if(m_render){
        m_render->unInit();
    }
    if(m_audio_out_buffer){
        delete m_audio_out_buffer;
        m_audio_out_buffer = nullptr;
    }
    if(m_swrContext){
        swr_free(&m_swrContext);
        m_swrContext = nullptr;
    }
}

void AudioDecoder::ClearCache() {
    if(m_render){
        m_render->clearAudioCache();
    }
}

long AudioDecoder::GetAudioDecoderStampForAVSync(void *context) {
    if(context){
        AudioDecoder* decoder = static_cast<AudioDecoder*>(context);
        return decoder->GetCurrentPosition();
    }
    return 0;
}
