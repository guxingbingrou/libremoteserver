/*
 * VideoEncoderFFmpeg.cpp
 *
 *  Created on: Nov 3, 2020
 *      Author: zou
 */

#include "VideoEncoderFFmpeg.h"
#include <unistd.h>
namespace RemoteServer {


VideoEncoderFFmpeg::VideoEncoderFFmpeg(VideoEncoderParams& params, std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& input_queue,
		std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& output_queue) {
	// TODO Auto-generated constructor stub
	m_params = params;
	m_input_queue = input_queue;
	m_output_queue = output_queue;
	InitVideoEncoder();
}

VideoEncoderFFmpeg::~VideoEncoderFFmpeg() {
	// TODO Auto-generated destructor stub
    DestroyVideoEncoder();
}

int VideoEncoderFFmpeg::InitVideoEncoder(){
    m_frame_count = 0;

    int ret;
    m_codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if(!m_codec){
    	SIMLOG(SimLogger::Error, "can not find codec");
        return -1;
    }

    m_code_context = avcodec_alloc_context3(m_codec);
    if(!m_code_context){
    	SIMLOG(SimLogger::Error,"can not allocate video codec context");
        return -1;
    }

    m_packet = av_packet_alloc();
    if (!m_packet){
    	SIMLOG(SimLogger::Error, "can not alloc av packet");
    	return -1;
    }

    SetVideoParams(m_params);

    m_frame = av_frame_alloc();
    if(!m_frame){
    	SIMLOG(SimLogger::Error, "can not allocate frame");
        return -1;
    }
    m_frame->format = m_code_context->pix_fmt;
    m_frame->width = m_code_context->width;
    m_frame->height = m_code_context->height;

//    if(!m_rgb_data){
//        m_rgb_data = (uint8_t*)malloc(m_frame->width * m_frame->height * 4);
//    }

    if(!m_yuv_data){
        int yuv_size = av_image_get_buffer_size(m_code_context->pix_fmt, m_frame->width, m_frame->height, 1);
        m_yuv_data = (unsigned char*)av_malloc(yuv_size);
        av_image_fill_arrays(m_frame->data, m_frame->linesize, m_yuv_data, AV_PIX_FMT_YUV420P, m_code_context->width, m_code_context->height, 1);
    }

    return 0;
}

int VideoEncoderFFmpeg::StartVideoEncoder(){
    if(m_start_encoder)
        return 0;

    if(avcodec_open2(m_code_context, m_codec, NULL) < 0){
    	SIMLOG(SimLogger::Error, "can not open codec");
        return -1;
    }
    m_start_encoder = true;
    m_input_queue->Start();
	m_output_queue->Start();
    m_process_thread = std::thread(&VideoEncoderFFmpeg::UpdateVideoEncoder, this);
    return 0;
}

int VideoEncoderFFmpeg::SetVideoParams(VideoEncoderParams& params){
    bool pre_start_encoder = m_start_encoder;
    if(m_start_encoder)
    	StopVideoEncoder();
    m_code_context->time_base.num = 1;

	m_code_context->bit_rate = params.bitrate;
	m_code_context->rc_min_rate = params.bitrate;
	m_code_context->rc_max_rate = params.bitrate;
	m_code_context->bit_rate_tolerance = params.bitrate;
	m_code_context->rc_buffer_size = params.bitrate;
	m_code_context->rc_initial_buffer_occupancy = m_code_context->rc_buffer_size*3/4;

//    if(m_video_call_back_ext->orientation % 2){
//        m_code_context->width = params.height;
//        m_code_context->height = params.width;
//    }
//    else{
        m_code_context->width = params.width;
        m_code_context->height = params.height;

//    }
    m_code_context->time_base = (AVRational){1, (int)(params.frameRate)};
    m_code_context->framerate = (AVRational){(int)(params.frameRate), 1};
    m_code_context->gop_size = params.gop_size;
    m_code_context->pix_fmt = AV_PIX_FMT_YUV420P;
    if(m_frame){
        m_frame->format = m_code_context->pix_fmt;
        m_frame->width = m_code_context->width;
        m_frame->height = m_code_context->height;
    }

    switch(params.profile){
    	case BASELINE:
    		m_code_context->profile = FF_PROFILE_H264_BASELINE;
    		break;
    	case MAIN:
    		m_code_context->profile = FF_PROFILE_H264_MAIN;
    		break;
    	case HIGH:
    		m_code_context->profile = FF_PROFILE_H264_HIGH;
    		break;
    	default:
    		m_code_context->profile = FF_PROFILE_H264_MAIN;
    		break;
    }

    av_opt_set(m_code_context->priv_data, "preset", "superfast", 0);
    av_opt_set(m_code_context->priv_data, "tune", "zerolatency", 0);

    m_params = params;
    if(pre_start_encoder)
    	StartVideoEncoder();
    return 0;
}

int VideoEncoderFFmpeg::UpdateVideoEncoder(){
    while(m_start_encoder){
        m_frame->pts = m_frame_count++;
        int ret = -1;

        // av_init_packet(m_packet);

        if(m_force_i_frame){
        	m_frame->pict_type = AV_PICTURE_TYPE_I;
        	m_force_i_frame = false;
			if(!m_input_queue->IsEmpty()){
				m_input_queue->WaitePopBuffer(m_input_buffer);
			}
	
        }
        else{
        	m_frame->pict_type = AV_PICTURE_TYPE_NONE;
        	m_input_queue->WaitePopBuffer(m_input_buffer);
        }

        RGB2YUV(m_input_buffer);
//        SIMLOG(SimLogger::Info, "RGB2YUV\n");

        ret = avcodec_send_frame(m_code_context, m_frame);
        if(ret < 0){
        	SIMLOG(SimLogger::Error, "encode error");
            return -1;
        }

        while(ret >=0){
            ret = avcodec_receive_packet(m_code_context, m_packet);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
            else if (ret < 0) {
            	SIMLOG(SimLogger::Error, "Error during encoding\n");
                break;
            }
            
    		std::vector<unsigned char> buffer(m_packet->data, m_packet->data + m_packet->size);
    		SIMLOG(SimLogger::Info, "m_packet->size " << m_packet->size);
    		m_output_queue->WaitePushBuffer(buffer);
            av_packet_unref(m_packet);
        }

    }
    SIMLOG(SimLogger::Info, "return " << m_start_encoder);
    return 0;
}

int VideoEncoderFFmpeg::StopVideoEncoder(){
	if(!m_start_encoder)
		return 0;
	m_start_encoder = false;
	if(m_process_thread.joinable())
		m_process_thread.join();
	m_input_queue->Stop();
	m_output_queue->Stop();
	if(m_code_context && avcodec_is_open(m_code_context))
		avcodec_close(m_code_context);
    SIMLOG(SimLogger::Info, "StopVideoEncoder OK");
	m_frame_count = 0;

	return 0;
}

int VideoEncoderFFmpeg::DestroyVideoEncoder(){
    StopVideoEncoder();
    if(m_code_context){
//    	avcodec_close(m_code_context);
    	av_free(m_code_context);
    	av_frame_free(&m_frame);
    	av_packet_free(&m_packet);
    	av_free(m_yuv_data);

    	m_codec = nullptr;
    	m_code_context = nullptr;
    	m_frame = nullptr;
    	m_packet = nullptr;
    	m_yuv_data = nullptr;

    	m_frame_count = 0;
    }
    return 0;
}

void VideoEncoderFFmpeg::RGB2YUV(std::vector<unsigned char>& rgb_data){
#if 1
    static int y_size = m_params.width * m_params.height;
	if(rgb_data.size() != y_size * 4)
		return;
    int y_index = 0;
    int u_index = y_size;
    int v_index = u_index + y_size / 4;
    int R, G, B, Y, U, V;

    for(int i=0; i<m_frame->height; i++){
        for(int j=0; j<m_frame->width; j++){
        	B = rgb_data[(i*m_frame->width + j) * 4 + 0];
        	G = rgb_data[(i*m_frame->width + j) * 4 + 1];
        	R = rgb_data[(i*m_frame->width + j) * 4 + 2];
        	Y = ((66*R + 129*G + 25*B) >> 8 ) + 16;
        	U = ((-38*R - 74*G + 112*B) >> 8 ) + 128;
        	V = ((112*R - 94*G - 18*B) >> 8 ) + 128;

        	m_yuv_data[y_index++] = (uint8_t)(Y);
        	if(i%2 == 0 && j%2 == 0){
        		m_yuv_data[u_index++] = (uint8_t)(U);
        		m_yuv_data[v_index++] = (uint8_t)(V);
        	}
        }
    }
    m_frame->data[0] = m_yuv_data;
    m_frame->data[1] = m_yuv_data + y_size;
    m_frame->data[2] = m_yuv_data + y_size * 5 / 4;
#endif

#if 0
	const int in_linesize[1] = {4 * m_code_context->width};
	m_sws_context = sws_getCachedContext(m_sws_context,
			m_code_context->width, m_code_context->height, AV_PIX_FMT_BGRA,
			m_code_context->width, m_code_context->height, AV_PIX_FMT_YUV420P,
			0, 0, 0, 0);
//	av_opt_show2(m_sws_context,NULL,AV_OPT_FLAG_VIDEO_PARAM,0);
	sws_scale(m_sws_context, (const uint8_t *const *)&rgb_data, in_linesize, 0, m_code_context->height, m_frame->data, m_frame->linesize);
#endif
}

int VideoEncoderFFmpeg::GenerateKeyFrame(){
	m_force_i_frame = true;
	return 0;
}

}
