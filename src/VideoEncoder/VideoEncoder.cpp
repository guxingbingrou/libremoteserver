//
// Created by zou on 11/5/20.
//
#include "VideoEncoder.h"
#include "VideoEncoderFFmpeg.h"

namespace RemoteServer{

std::shared_ptr<VideoEncoder> VideoEncoder::m_video_encoder;
std::mutex VideoEncoder::m_mutex;

std::shared_ptr<VideoEncoder> VideoEncoder::CreateVideoEncoder(VideoEncoderType encoder_type,
		VideoEncoderParams& params, std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& input_queue,
        std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& output_queue){
    if(!m_video_encoder){
        std::unique_lock<std::mutex> lck(m_mutex);
        if(!m_video_encoder){
            switch(encoder_type){
                case TYPE_FFMPEG:
                    m_video_encoder = std::make_shared<VideoEncoderFFmpeg>(params, input_queue, output_queue);
                    break;
                default:
                    SIMLOG(SimLogger::Error, "unknown encoder type!");
                    break;
            }

        }
    }
    return m_video_encoder;
}

VideoEncoder::VideoEncoder(){

}

VideoEncoder::~VideoEncoder(){

}

}
