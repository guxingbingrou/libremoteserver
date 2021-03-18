/*
 * FFmepgVideoEncoderTest.cpp
 *
 *  Created on: Jan 4, 2021
 *      Author: zou
 */

#include "VideoCapture/VideoCapture.h"
#include "VideoEncoder/VideoEncoder.h"
#include "Logger/Logger.h"
#include <fstream>
#include <memory>
#include <unistd.h>
#include "../../src/Base/BufferQueue/BufferQueue.hpp"

using namespace RemoteServer;
int main(){
	SimLogger::Logger::CreateLoggerInstance();
	SimLogger::Logger::InitLogger(SimLogger::ConsoleType, "LoggerTest", SimLogger::Info);

	std::ofstream file_stream("/home/zou/x11encoder.h264", std::ios::out | std::ios::binary);

	auto buffer_queue_rgb = std::make_shared< BufferQueue<unsigned char> >(5);

	auto buffer_queue_h264 = std::make_shared< BufferQueue<unsigned char> >(5);

	auto video_capture = VideoCapture::CreateVideoCapture(X11Desktop, buffer_queue_rgb);

	VideoEncoderParams param;
	auto format = video_capture->GetVideoFormat();
	param.height = format->height;
	param.width = format->width;

	auto video_encoder = VideoEncoder::CreateVideoEncoder(TYPE_FFMPEG, param, buffer_queue_rgb, buffer_queue_h264);

	video_capture->StartVideoCapture();
	video_encoder->StartVideoEncoder();
	int count = 0;
	while(count++ < 500){
		std::vector<unsigned char> buffer;
		buffer_queue_h264->WaitePopBuffer(buffer);
		file_stream.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
//		usleep(20000);
	}

	video_encoder->StopVideoEncoder();
	video_capture->StopVideoCapture();
	file_stream.close();
//	delete[] data;
	video_encoder->DestroyVideoEncoder();
	video_capture->DestroyVideoCapture();
	return 0;
}



