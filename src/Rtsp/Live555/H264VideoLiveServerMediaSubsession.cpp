/*
 * H264VideoLiveServerMediaSubsession.cpp
 *
 *  Created on: Dec 31, 2020
 *      Author: zou
 */

#include "H264VideoLiveServerMediaSubsession.hh"
#include "H264VideoStreamFramer.hh"
#include "H264VideoRTPSink.hh"
H264VideoLiveServerMediaSubsession*
H264VideoLiveServerMediaSubsession::createNew(UsageEnvironment& env, std::shared_ptr<BufferQueue::BufferQueue<unsigned char>> & buffer_queue){
	return new H264VideoLiveServerMediaSubsession(env, buffer_queue);
}

H264VideoLiveServerMediaSubsession::H264VideoLiveServerMediaSubsession(UsageEnvironment& env, std::shared_ptr<BufferQueue::BufferQueue<unsigned char>> & buffer_queue)
: OnDemandServerMediaSubsession(env, False)  {
	// TODO Auto-generated constructor stub
	m_buffer_queue = buffer_queue;
}

H264VideoLiveServerMediaSubsession::~H264VideoLiveServerMediaSubsession() {
	// TODO Auto-generated destructor stub
	if(m_sdp_line){
		delete[] m_sdp_line;
		m_sdp_line = nullptr;
	}
}

void H264VideoLiveServerMediaSubsession::afterPlayingDummy(void* clientData) {
	H264VideoLiveServerMediaSubsession* subsess = (H264VideoLiveServerMediaSubsession*)clientData;
	subsess->afterPlayingDummy1();
}

void H264VideoLiveServerMediaSubsession::afterPlayingDummy1() {
  // Unschedule any pending 'checking' task:
	envir().taskScheduler().unscheduleDelayedTask(nextTask());
  // Signal the event loop that we're done:
	setDoneFlag();
}

void H264VideoLiveServerMediaSubsession::checkForAuxSDPLine(void* clientData) {
	H264VideoLiveServerMediaSubsession* subsess = (H264VideoLiveServerMediaSubsession*)clientData;
	subsess->checkForAuxSDPLine1();
}

void H264VideoLiveServerMediaSubsession::checkForAuxSDPLine1() {
	nextTask() = NULL;

	char const* dasl;
	if (m_sdp_line) {
    // Signal the event loop that we're done:
		setDoneFlag();
	}
	else if (m_dummy_rtp_sink  && (dasl = m_dummy_rtp_sink->auxSDPLine()) != NULL) {
		m_sdp_line = strDup(dasl);

//		m_dummy_rtp_sink->stopPlaying();
		m_dummy_rtp_sink = NULL;

    // Signal the event loop that we're done:
		setDoneFlag();
	} else if (!m_done) {
    // try again after a brief delay:
		int uSecsToDelay = 100000; // 100 ms
		nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
			      (TaskFunc*)checkForAuxSDPLine, this);
	}
}
char const* H264VideoLiveServerMediaSubsession::getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource) {
	if (m_sdp_line) return m_sdp_line; // it's already been set up (for a previous client)

	if (!m_dummy_rtp_sink) { // we're not already setting it up for another, concurrent stream
    // Note: For H264 video files, the 'config' information ("profile-level-id" and "sprop-parameter-sets") isn't known
    // until we start reading the file.  This means that "rtpSink"s "auxSDPLine()" will be NULL initially,
    // and we need to start reading data from our file until this changes.
		m_dummy_rtp_sink = rtpSink;

    // Start reading the file:
//		m_dummy_rtp_sink->startPlaying(*inputSource, afterPlayingDummy, this);
		m_dummy_rtp_sink->startPlaying(*inputSource, 0, 0);
    // Check whether the sink's 'auxSDPLine()' is ready:
		checkForAuxSDPLine(this);

		envir().taskScheduler().doEventLoop(&m_done);

	}

	return m_sdp_line;
}

FramedSource* H264VideoLiveServerMediaSubsession::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate) {

  // Create a framer for the Video Elementary Stream
	estBitrate = 500;
  return H264VideoStreamFramer::createNew(envir(), new H264LiveVideoSource(envir(), m_buffer_queue));
}

RTPSink* H264VideoLiveServerMediaSubsession
::createNewRTPSink(Groupsock* rtpGroupsock,
		   unsigned char rtpPayloadTypeIfDynamic,
		   FramedSource* /*inputSource*/) {
  return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}

