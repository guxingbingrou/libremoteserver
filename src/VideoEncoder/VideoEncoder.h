/*
 * VideoEncoder.h
 *
 *  Created on: Nov 3, 2020
 *      Author: zou
 */

#ifndef VIDEOENCODER_VIDEOENCODER_H_
#define VIDEOENCODER_VIDEOENCODER_H_
#include "Logger/Logger.h"
#include "../Base/BufferQueue/BufferQueue.hpp"
#include <mutex>
#include <memory>
#include <thread>

namespace RemoteServer {


enum ProfileType{
	BASELINE,
	MAIN,
	HIGH
};
struct VideoEncoderParams{
	int width = 1920;
	int height = 1080;
	int bitrate = 5000000;
	int frameRate = 25;
	int gop_size = 50;
	ProfileType profile = BASELINE;
};
class VideoEncoder {
public:
	virtual int StartVideoEncoder() = 0;

	virtual int SetVideoParams(VideoEncoderParams& videoParams) = 0;

	virtual int StopVideoEncoder() = 0;

	virtual int DestroyVideoEncoder() = 0;

	virtual int GenerateKeyFrame() = 0;

	VideoEncoder()=default;

	virtual ~VideoEncoder()=default;

protected:
	VideoEncoderParams m_params;
	std::shared_ptr<BufferQueue::BufferQueue<unsigned char>> m_input_queue;
	std::shared_ptr<BufferQueue::BufferQueue<unsigned char>> m_output_queue;

};

}
#endif /* VIDEOENCODER_VIDEOENCODER_H_ */
