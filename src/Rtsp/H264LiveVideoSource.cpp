/*
 * H264LiveVideoSource.cpp
 *
 *  Created on: Dec 31, 2020
 *      Author: zou
 */

#include "H264LiveVideoSource.hh"

H264LiveVideoSource::H264LiveVideoSource(UsageEnvironment& env, std::shared_ptr<RemoteServer::BufferQueue<unsigned char>>&  buffer_queue)
	:FramedSource(env),
	 m_buffer_queue(buffer_queue) {
	// TODO Auto-generated constructor stub

}

H264LiveVideoSource::~H264LiveVideoSource() {
	// TODO Auto-generated destructor stub
	printf("H264LiveVideoSource::~H264LiveVideoSource()\n");
	fflush(stdout);
	if(m_data){
		free(m_data);
		m_data = nullptr;

	}
}
//unsigned int H264LiveVideoSource::maxFrameSize() const
//{
//    return 102400;
//}

void H264LiveVideoSource::doStopGettingFrames(){
	m_left=0;
}
void H264LiveVideoSource::doGetNextFrame() {
    // 根据 fps，计算等待时间
    envir().taskScheduler().scheduleDelayedTask(0, getNextFrame, this);
}

void H264LiveVideoSource::getNextFrame(void * ptr)
{
    ((H264LiveVideoSource *)ptr)->GetFrameData();
}

void H264LiveVideoSource::GetFrameData()
{
    gettimeofday(&fPresentationTime, 0);
    fFrameSize = 0;
//    static std::vector<unsigned char> buffer;
#if 1
	if(m_left == 0){
    	m_buffer_queue->WaitePopBuffer(m_buffer);
    	m_left = m_buffer.size();
	}
	if(fMaxSize <= m_left){
		std::copy(m_buffer.end() - m_left, m_buffer.end() - m_left + fMaxSize, fTo);
//		memcpy(fTo, m_data, fMaxSize);
		fFrameSize = fMaxSize;
//		memmove(m_data, m_data+fMaxSize, m_left-fMaxSize);
		m_left -= fMaxSize;
	}
	else {
//			printf("H264LiveVideoSource222::GetFrameData left=%d, need=%d\n " , left, need);
		std::copy(m_buffer.end() - m_left, m_buffer.end(), fTo);
//		memcpy(fTo, m_data, m_left);
		fFrameSize = m_left;
		m_left = 0;
	}

#endif

#if 0
    if(!m_data)
    	m_data = (unsigned char*)malloc(600000);
   // static unsigned char* data = (unsigned char*)malloc(600000);
 //   static FILE* file = fopen("/home/zou/test.h264", "wb");
	if(m_left == 0){
    	m_buffer_queue->WaitePopBuffer(buffer);
    	std::copy(buffer.begin(), buffer.end(), m_data);
    	m_left = buffer.size();
	}
	if(fMaxSize <= m_left){
//			printf("H264LiveVideoSource111::GetFrameData left=%d, need=%d\n " , left, need);
		memcpy(fTo, m_data, fMaxSize);
		fFrameSize = fMaxSize;
		memmove(m_data, m_data+fMaxSize, m_left-fMaxSize);
		m_left -= fMaxSize;
	}
	else {
//			printf("H264LiveVideoSource222::GetFrameData left=%d, need=%d\n " , left, need);
		memcpy(fTo, m_data, m_left);
		fFrameSize = m_left;
		m_left = 0;
	}

#endif
//    fwrite(fTo, 1, fFrameSize, file);
//    printf("H264LiveVideoSource111::GetFrameData    buffer.size()=%d, fFrameSize=%d, fMaxSize=%d\n " , buffer.size(), fFrameSize,fMaxSize );
    afterGetting(this);
//    printf("H264LiveVideoSource2222::GetFrameData    buffer.size()=%d, fFrameSize=%d, fMaxSize=%d\n " , buffer.size(), fFrameSize,fMaxSize );

}
