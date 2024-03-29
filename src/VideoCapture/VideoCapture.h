/*
 * VideoCapture.h
 *
 *  Created on: Apr 3, 2020
 *      Author: zou
 */

#ifndef SRC_VIDEOCAPTURE_VIDEOCAPTURE_H_
#define SRC_VIDEOCAPTURE_VIDEOCAPTURE_H_
#include <memory>
#include <mutex>
#include <thread>

#include "../Base/BufferQueue/BufferQueue.hpp"
#include "Logger/Logger.h"

namespace RemoteServer {
enum PixelType{
	RGB888,
	BGR888,
	BRG888,
	RGBA8888,
	YUV420,
	YUV422,
	YUV444
};

struct VideoFormat{
	int width;
	int height;
	int bits_per_pixel;
	int depth;
	PixelType pixel_type;//like RGBA8888 YUV420...
	int capture_fps;
};
class VideoCapture {
public:
	VideoCapture();
	virtual ~VideoCapture();

	inline std::shared_ptr<VideoFormat> GetVideoFormat(){return m_video_format;}

	virtual void UpdateVideoCapture() = 0;

    virtual bool StartVideoCapture();

    virtual bool StopVideoCapture();

	virtual bool DestroyVideoCapture() = 0;

protected:
	std::shared_ptr<VideoFormat> m_video_format;
	bool m_start_capture = false;
	std::thread m_process_thread;
	std::shared_ptr<BufferQueue::BufferQueue<unsigned char>> m_buffer_queue;

};

} /* namespace RemoteServer */

#endif /* SRC_VIDEOCAPTURE_VIDEOCAPTURE_H_ */
