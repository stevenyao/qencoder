#include "stable.h"

extern "C"
{
	#include "x264.h"
}

#include "VideoEncoder_x264.h"

VideoEncoder_x264::VideoEncoder_x264()
{
	x264_param_t param = {0};
	x264_param_default_preset( &param, "veryfast", "psnr" );
	param.rc.i_rc_method = X264_RC_ABR;
	param.rc.i_bitrate = 800;
	param.rc.i_vbv_buffer_size = param.rc.i_bitrate * 1.2;
	param.rc.i_vbv_max_bitrate = param.rc.i_bitrate;

	param.i_width = 1280;
	param.i_height = 720;
	//param.i_fps_num
	m_context = x264_encoder_open(&param);

	x264_encoder_reconfig(m_context, &param);
}

VideoEncoder_x264::~VideoEncoder_x264()
{
	x264_encoder_close(m_context);
}

int VideoEncoder_x264::encode(const VideoSample &sampleIn)
{
	x264_picture_t picIn;
	x264_picture_t picOut;

	x264_picture_init(&picIn);
	x264_picture_alloc(&picIn, X264_CSP_I420, 1280, 720);

	memcpy(picIn.img.plane[0], sampleIn.plane(0), sampleIn.width() * sampleIn.height());
	memcpy(picIn.img.plane[1], sampleIn.plane(1), sampleIn.width() * sampleIn.height() / 4);
	memcpy(picIn.img.plane[2], sampleIn.plane(2), sampleIn.width() * sampleIn.height() / 4);

	picIn.i_pts = sampleIn.pts();

	x264_nal_t *nalOut = 0;
	int nalNum = 0;
	int r = x264_encoder_encode(m_context, &nalOut, &nalNum, &picIn, &picOut);

	if(r > 0) {
		VideoEncodedSample *sampleOut = new VideoEncodedSample();
		sampleOut->setData(nalOut->p_payload, r);
		m_output.push_back(sampleOut);

		return 1;
	}

	return 0;
}

VideoEncodedSample *VideoEncoder_x264::getOutput()
{
	if(m_output.size() > 0) {
		VideoEncodedSample *sampleOut = m_output.front();
		m_output.pop_front();
		return sampleOut;
	}

	return NULL;
}
