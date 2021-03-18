/*
 * H264VideoLiveServerMediaSubsession.hh
 *
 *  Created on: Dec 31, 2020
 *      Author: zou
 */

#ifndef LIVEMEDIA_H264VIDEOLIVESERVERMEDIASUBSESSION_HH_
#define LIVEMEDIA_H264VIDEOLIVESERVERMEDIASUBSESSION_HH_
//#include "liveMedia.hh"
//#include "BasicUsageEnvironment.hh"
//#include "GroupsockHelper.hh"
#include "OnDemandServerMediaSubsession.hh"
#include "ServerMediaSession.hh"
#include "BufferQueue.hpp"
#include "H264LiveVideoSource.hh"
#include <memory>
class H264VideoLiveServerMediaSubsession: public OnDemandServerMediaSubsession {
public:
	static H264VideoLiveServerMediaSubsession*
		createNew(UsageEnvironment& env, std::shared_ptr<RemoteServer::BufferQueue<unsigned char>> & buffer_queue);


	  // Used to implement "getAuxSDPLine()":
	  static void checkForAuxSDPLine(void * ptr);
	  void checkForAuxSDPLine1();
	  static void afterPlayingDummy(void * ptr);
	  void afterPlayingDummy1();
	  void setDoneFlag() { m_done = ~0; }

protected:
	  H264VideoLiveServerMediaSubsession(UsageEnvironment& env, std::shared_ptr<RemoteServer::BufferQueue<unsigned char>> & buffer_queue);
	      // called only by createNew();
	  virtual ~H264VideoLiveServerMediaSubsession();




	  virtual char const* getAuxSDPLine(RTPSink* rtpSink,
					    FramedSource* inputSource);
	  virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
						      unsigned& estBitrate);
	  virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
	                                    unsigned char rtpPayloadTypeIfDynamic,
					    FramedSource* inputSource);



private:
	std::shared_ptr<RemoteServer::BufferQueue<unsigned char>>  m_buffer_queue;
	char* m_sdp_line = nullptr;
	char m_done; // used when setting up "fAuxSDPLine"
	RTPSink* m_dummy_rtp_sink = nullptr; // ditto
};

#endif /* LIVEMEDIA_H264VIDEOLIVESERVERMEDIASUBSESSION_HH_ */
