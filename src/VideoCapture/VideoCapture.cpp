/*
 * VideoCapture.cpp
 *
 *  Created on: Apr 3, 2020
 *      Author: zou
 */

#include "VideoCapture.h"
#include "X11DesktopCapture.h"
namespace RemoteServer {

std::mutex VideoCapture::m_mutex;
std::shared_ptr<VideoCapture> VideoCapture::m_video_capture;
VideoCapture::VideoCapture() {
	// TODO Auto-generated constructor stub
	m_video_format = std::make_shared<VideoFormat>();
}
std::shared_ptr<VideoCapture> VideoCapture::CreateVideoCapture(VideoCaptureType type, std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& buffer_queue){
	if(!m_video_capture){
		std::unique_lock<std::mutex> lock(m_mutex);
		if(!m_video_capture){
			switch(type){
				case X11Desktop:
					m_video_capture = std::make_shared<X11DesktopCapture>(buffer_queue);
					break;
				default:
					SIMLOG(SimLogger::Error, "unknow videocapture type");
					break;
			}
		}
	}

	return m_video_capture;

}
VideoCapture::~VideoCapture() {
	// TODO Auto-generated destructor stub
}

bool VideoCapture::StartVideoCapture(){
	m_start_capture = true;
	m_process_thread = std::thread(&VideoCapture::UpdateVideoCapture, m_video_capture);
	return true;
}
bool VideoCapture::StopVideoCapture(){
	m_start_capture = false;
	m_buffer_queue->SetWaitePush(false);
	if(m_process_thread.joinable())
		m_process_thread.join();
	return true;
}

} /* namespace RemoteServer */
