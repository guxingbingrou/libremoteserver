#ifndef __REMOTE_SERVER_VIDEO_ENCODER_FFMPEG_FACTORY__
#define __REMOTE_SERVER_VIDEO_ENCODER_FFMPEG_FACTORY__
#include "VideoEncoderFactory.h"
#include "VideoEncoderFFmpeg.h"
namespace RemoteServer{


class VideoEncoderFFmpegFactory:public VideoEncoderFactory
{
private:
    /* data */
public:
    VideoEncoderFFmpegFactory(/* args */)=default;
    ~VideoEncoderFFmpegFactory()=default;

    std::shared_ptr<VideoEncoder> CreateVideoEncoder(
			VideoEncoderParams& params, std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& input_queue, 
            std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& output_queue) final{
        return std::make_shared<VideoEncoderFFmpeg>(params, input_queue, output_queue);
    }
};


}

#endif