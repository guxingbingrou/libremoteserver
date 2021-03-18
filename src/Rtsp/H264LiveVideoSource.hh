/*
 * H264LiveVideoSource.h
 *
 *  Created on: Dec 31, 2020
 *      Author: zou
 */

#ifndef LIVEMEDIA_H264LIVEVIDEOSOURCE_HH_
#define LIVEMEDIA_H264LIVEVIDEOSOURCE_HH_

#include "FramedSource.hh"
//#include "liveMedia.hh"
//#include "BasicUsageEnvironment.hh"
//#include "GroupsockHelper.hh"
#include "BufferQueue.hpp"
#include <vector>
class H264LiveVideoSource: public FramedSource {
public:
	H264LiveVideoSource(UsageEnvironment& env, std::shared_ptr<RemoteServer::BufferQueue<unsigned char>>&  buffer_queue);
	virtual ~H264LiveVideoSource();

private:
	std::shared_ptr<RemoteServer::BufferQueue<unsigned char>>  m_buffer_queue;

	  // redefined virtual functions:
	virtual void doGetNextFrame();
	static void getNextFrame(void * ptr);

	void GetFrameData() ;

	virtual void doStopGettingFrames();
//	virtual unsigned maxFrameSize() const;
	std::vector<unsigned char> m_buffer;
	unsigned char* m_data = nullptr;
	int m_left = 0;
};

#endif /* LIVEMEDIA_H264LIVEVIDEOSOURCE_HH_ */
