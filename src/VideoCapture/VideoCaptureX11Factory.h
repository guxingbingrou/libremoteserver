#ifndef __REMOTE_SERVER_VIDEO_CAPTURE_X11_FACTORY__
#define __REMOTE_SERVER_VIDEO_CAPTURE_X11_FACTORY__
#include "VideoCaptureFactory.h"
#include "X11DesktopCapture.h"
namespace RemoteServer{


class VideoCaptureX11Factory:public VideoCaptureFactory
{
private:
    /* data */
public:
    VideoCaptureX11Factory(/* args */)=default;
    ~VideoCaptureX11Factory()=default;

    std::shared_ptr<VideoCapture> CreateVideoCapture(std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& buffer_queue) final{
        return std::make_shared<X11DesktopCapture>(buffer_queue);
    }
};


}

#endif