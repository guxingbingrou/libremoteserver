#ifndef __REMOTE_SERVER_VIDEO_CAPTURE_FACTORY_H_
#define __REMOTE_SERVER_VIDEO_CAPTURE_FACTORY_H_
#include "VideoCapture.h"
namespace RemoteServer {
class VideoCaptureFactory
{
private:
    /* data */
public:
    VideoCaptureFactory(/* args */)=default;
    virtual ~VideoCaptureFactory()=default;
    VideoCaptureFactory(const VideoCaptureFactory& ) = delete;
    VideoCaptureFactory(VideoCaptureFactory&& ) = delete;
    VideoCaptureFactory& operator=(const VideoCaptureFactory&) =delete;
    VideoCaptureFactory& operator=(VideoCaptureFactory &&) = delete;

    virtual std::shared_ptr<VideoCapture> CreateVideoCapture(std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& buffer_queue)=0;

};

}

#endif