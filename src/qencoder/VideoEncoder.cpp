#include "stable.h"

#include "VideoEncoder.h"
#include "VideoEncoder_x264.h"

VideoSample::VideoSample()
{
	memset(this, 0, sizeof(*this));
}

VideoSample::~VideoSample()
{
}

bool VideoSample::setData(int width, int height, int csp, int numOfPlane, int stride[], uint8_t *plane[], int64_t pts)
{
	m_width = width;
	m_height = height;
	m_csp = csp;
	m_numOfPlane = numOfPlane;
	m_pts = pts;
	for(int i = 0; i < numOfPlane; i++) {
		m_stride[i] = stride[i];
		m_plane[i] = plane[i];
	}

	return true;
}

VideoEncodedSample::VideoEncodedSample()
	: m_size(0)
	, m_data(NULL)
{

}

void VideoEncodedSample::setData(const uint8_t *data, int size)
{
	m_data = new uint8_t[size];
	memcpy(m_data, data, size);
	m_size = size;
}

VideoEncoder::VideoEncoder()
{

}

VideoEncoder::~VideoEncoder()
{

}

int VideoEncoder::encode(const VideoSample &sampleIn)
{
	Q_UNUSED(sampleIn);
	return 0;
}

VideoEncodedSample *VideoEncoder::getOutput()
{
	return NULL;
}

VideoEncoder *VideoEncoder::create(const char *codecName)
{
	if(strcmp(codecName, "x264") == 0) {
		return new VideoEncoder_x264();
	}

	return NULL;
}
