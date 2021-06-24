/*
 * VideoEncoderFFmpeg.h
 *
 *  Created on: Nov 3, 2020
 *      Author: zou
 */

#ifndef VIDEOENCODER_VIDEOENCODERFFMPEG_H_
#define VIDEOENCODER_VIDEOENCODERFFMPEG_H_

#include "VideoEncoder.h"
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
}
namespace RemoteServer {


class VideoEncoderFFmpeg: public VideoEncoder {
public:
	VideoEncoderFFmpeg(VideoEncoderParams& params, std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& input_queue,
			std::shared_ptr<BufferQueue::BufferQueue<unsigned char>>& output_queue);


	int StartVideoEncoder() final;

	int SetVideoParams(VideoEncoderParams& params) final;

	int UpdateVideoEncoder() ;

	int StopVideoEncoder() final;

	int DestroyVideoEncoder() final;

	int GenerateKeyFrame() final;

	virtual ~VideoEncoderFFmpeg();

private:
	int InitVideoEncoder();

	void RGB2YUV(std::vector<unsigned char>& input_buffer);

	unsigned char* m_yuv_data = nullptr;
	std::vector<unsigned char> m_input_buffer;

	const AVCodec* m_codec = nullptr;

	AVCodecContext* m_code_context = nullptr;

	AVFrame* m_frame = nullptr;

	AVPacket* m_packet = nullptr;

	SwsContext* m_sws_context = nullptr;

	int m_frame_count = 0;

	bool m_force_i_frame = false;
 	
	bool m_start_encoder = false;

	std::thread m_process_thread;
};

}
#endif /* VIDEOENCODER_VIDEOENCODERFFMPEG_H_ */
