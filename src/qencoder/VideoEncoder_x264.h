#pragma once

#include "VideoEncoder.h"

struct x264_t;

class VideoEncoder_x264 : public VideoEncoder
{
public:
	VideoEncoder_x264();
	~VideoEncoder_x264();

	int encode(const VideoSample &sampleIn);
	VideoEncodedSample *getOutput();

private:
	x264_t *m_context;

	std::list<VideoEncodedSample *> m_output;
};