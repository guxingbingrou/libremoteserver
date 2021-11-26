/*
 * VideoCapture.cpp
 *
 *  Created on: Apr 3, 2020
 *      Author: zou
 */

#include "VideoCapture.h"
#include "X11DesktopCapture.h"
namespace RemoteServer {


VideoCapture::VideoCapture() {
	// TODO Auto-generated constructor stub
	m_video_format = std::make_shared<VideoFormat>();
}

VideoCapture::~VideoCapture() {
	// TODO Auto-generated destructor stub
}

bool VideoCapture::StartVideoCapture(){
	if(m_start_capture)
		return true;
	m_start_capture = true;
	m_buffer_queue->Start();
	m_process_thread = std::thread(&VideoCapture::UpdateVideoCapture, this);
	return true;
}
bool VideoCapture::StopVideoCapture(){
	if(!m_start_capture)
		return true;
	m_start_capture = false;
	m_buffer_queue->Stop();
	if(m_process_thread.joinable())
		m_process_thread.join();
	SIMLOG(SimLogger::Info, "StopVideoCapture OK");
	return true;
}

} /* namespace RemoteServer */
