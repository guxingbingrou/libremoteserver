#ifndef __REMOTE_SERVER_VIDEO_ENCODER_FACTORY__
#define __REMOTE_SERVER_VIDEO_ENCODER_FACTORY__
#include "VideoEncoder.h"
namespace RemoteServer {
class VideoEncoderFactory
{
private:
    /* data */
public:
    VideoEncoderFactory(/* args */)=default;
    virtual ~VideoEncoderFactory()=default;
    VideoEncoderFactory(const VideoEncoderFactory& ) = delete;
    VideoEncoderFactory(VideoEncoderFactory&& ) = delete;
    VideoEncoderFactory& operator=(const VideoEncoderFactory&) =delete;
    VideoEncoderFactory& operator=(VideoEncoderFactory &&) = delete;

    virtual std::shared_ptr<VideoEncoder> CreateVideoEncoder(
			VideoEncoderParams& params, std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& input_queue, 
            std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& output_queue)=0;

};

}

#endif