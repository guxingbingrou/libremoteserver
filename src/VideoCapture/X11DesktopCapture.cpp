/*
 * X11DesktopCapture.cpp
 *
 *  Created on: Apr 3, 2020
 *      Author: zou
 */

#include "X11DesktopCapture.h"
#include <cstring>
namespace RemoteServer {

X11DesktopCapture::X11DesktopCapture(std::shared_ptr<BufferQueue<unsigned char>>& buffer_queue)
{
	// TODO Auto-generated constructor stub
	m_buffer_queue = buffer_queue;
	InitVideoCapture();
}

X11DesktopCapture::~X11DesktopCapture() {
	// TODO Auto-generated destructor stub
	DestroyVideoCapture();
}

bool X11DesktopCapture::InitVideoCapture(){
	m_display = XOpenDisplay(NULL);
	if(!m_display){
		SIMLOG(SimLogger::Error, "can not connect a display");
		return false;
	}
	m_desktop_window = RootWindow(m_display, 0);
	if(!m_desktop_window){
		SIMLOG(SimLogger::Error, "can not get the root window");
		return false;
	}
	m_video_format->pixel_type = RGBA8888;
	m_video_format->width = DisplayWidth(m_display, 0);
	m_video_format->height = DisplayHeight(m_display, 0);
	XImage* window_image = XGetImage(m_display, m_desktop_window, 0, 0, m_video_format->width, m_video_format->height, AllPlanes, ZPixmap);
	m_video_format->bits_per_pixel = window_image->bits_per_pixel;
	m_video_format->depth = window_image->depth;
	m_screen_size = m_video_format->width * m_video_format->height * m_video_format->bits_per_pixel / 8;
	SIMLOG(SimLogger::Info, "w/h/d/bpp/size: " << m_video_format->width <<"/" << m_video_format->height << "/"
			<< m_video_format->depth << "/" << m_video_format->bits_per_pixel << "/" <<  m_screen_size);
	XDestroyImage(window_image);

	//create share memory image
	return InitShmImages();
}
bool X11DesktopCapture::InitShmImages(){
	m_shm_segment_info = new XShmSegmentInfo();
	m_shm_segment_info->shmid = -1;
	m_x_shm_image = XShmCreateImage(m_display,
		      DefaultVisual(m_display,0), // Use a correct visual. Omitted for brevity
			  m_video_format->depth,   // Determine correct depth from the visual. Omitted for brevity
		      ZPixmap, NULL, m_shm_segment_info, m_video_format->width, m_video_format->height);
	if(!m_x_shm_image){
		SIMLOG(SimLogger::Error, "can not create XshmImage");
		return false;
	}
	m_shm_segment_info->shmid = shmget(IPC_PRIVATE,
			m_x_shm_image->bytes_per_line * m_x_shm_image->height,
		      IPC_CREAT|0777);
	if(m_shm_segment_info->shmid == -1){
		SIMLOG(SimLogger::Error, "shmid failed");
		return false;
	}
	m_shm_segment_info->shmaddr = m_x_shm_image->data = (char* )shmat(m_shm_segment_info->shmid, 0, 0);
//	m_shm_segment_info->readOnly = False;
	if(!XShmAttach(m_display, m_shm_segment_info)){
		SIMLOG(SimLogger::Error, "XShmAttach failed");
		return false;
	}
	SIMLOG(SimLogger::Debug, "initShmImages success");
	return true;
}

void X11DesktopCapture::UpdateVideoCapture(){
	while(m_start_capture){
		if(!XShmGetImage(m_display, m_desktop_window, m_x_shm_image, 0, 0, AllPlanes)){
			SIMLOG(SimLogger::Error, "update shmImage failed");
			return ;
		}
		std::vector<unsigned char> buffer(m_x_shm_image->data, m_x_shm_image->data + m_screen_size);
		m_buffer_queue->WaitePushBuffer(buffer);
	}

//	memcpy(video_data, m_x_shm_image->data, m_screen_size );
}
bool X11DesktopCapture::DestroyVideoCapture(){
	StopVideoCapture();
	if(m_shm_segment_info && m_shm_segment_info->shmid != -1 && m_display){
		if(!XShmDetach(m_display, m_shm_segment_info)){
			SIMLOG(SimLogger::Error, "XShmDetach failed");
			return false;
		}
	}
	if(m_x_shm_image){
		XDestroyImage(m_x_shm_image);
		m_x_shm_image = nullptr;
	}
	if(m_shm_segment_info && m_shm_segment_info->shmaddr != (char*)-1 && m_display){
		shmdt(m_shm_segment_info->shmaddr);
	}
	if(m_shm_segment_info && m_shm_segment_info->shmid != -1 && m_display){
		shmctl(m_shm_segment_info->shmid, IPC_RMID, 0);
		m_display = nullptr;
	}
	if(m_shm_segment_info){
		m_shm_segment_info->shmaddr = (char*)-1;
		m_shm_segment_info->shmid = -1;
		delete m_shm_segment_info;
		m_shm_segment_info = nullptr;
	}

	return true;
}


} /* namespace RemoteServer */
