#include "VideoCapture/VideoCapture.h"
#include "VideoCapture/VideoCaptureFactory.h"
#include "VideoCapture/VideoCaptureX11Factory.h"
#include "Logger/Logger.h"
#include <fstream>
#include <memory>
#include <unistd.h>
#include "../../src/Base/BufferQueue/BufferQueue.hpp"

using namespace RemoteServer;
int main(){
	SimLogger::Logger::CreateLoggerInstance();
	SimLogger::Logger::InitLogger(SimLogger::ConsoleType, "LoggerTest", SimLogger::Info);

	std::ofstream file_stream("x11capture.rgb", std::ios::out | std::ios::binary);

	auto buffer_queue = std::make_shared< BufferQueue::BufferQueue<unsigned char> >(5);

	auto x11_factory = std::make_unique<VideoCaptureX11Factory>();
	auto video_capture = x11_factory->CreateVideoCapture(buffer_queue);

	auto format = video_capture->GetVideoFormat();
	int screen_size = format->width * format->height * format->bits_per_pixel / 8;
//	unsigned char* data = new unsigned char[screen_size];
	video_capture->StartVideoCapture();
	int count = 0;
	while(count++ < 500){
		std::vector<unsigned char> buffer;
		buffer_queue->WaitePopBuffer(buffer);
		file_stream.write(reinterpret_cast<char*>(buffer.data()), screen_size);
		usleep(20000);
	}
	video_capture->StopVideoCapture();
	file_stream.close();
//	delete[] data;
	video_capture->DestroyVideoCapture();
	return 0;
}
