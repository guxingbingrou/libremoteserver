/*
 * X11DesktopCapture.h
 *
 *  Created on: Apr 3, 2020
 *      Author: zou
 */

#ifndef SRC_VIDEOCAPTURE_X11DESKTOPCAPTURE_H_
#define SRC_VIDEOCAPTURE_X11DESKTOPCAPTURE_H_

#include "VideoCapture/VideoCapture.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <sys/shm.h>
#include <string>
#include <vector>
namespace RemoteServer {

class X11DesktopCapture: public VideoCapture {
public:
	X11DesktopCapture(std::shared_ptr<BufferQueue<unsigned char>>& buffer_queue);
	virtual ~X11DesktopCapture();

	void UpdateVideoCapture() final;

//	bool StopVideoCapture() final;

	bool DestroyVideoCapture() final;

private:
	bool InitVideoCapture();

    bool InitShmImages();
    Display* m_display = nullptr;
    Window m_desktop_window = 0;
	XShmSegmentInfo* m_shm_segment_info = nullptr;
	XImage* m_x_shm_image = nullptr;
	int m_screen_size = 0;

};

} /* namespace RemoteServer */

#endif /* SRC_VIDEOCAPTURE_X11DESKTOPCAPTURE_H_ */
