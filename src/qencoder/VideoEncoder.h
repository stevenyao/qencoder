#pragma once

class VideoSample
{
public:
	VideoSample();
	~VideoSample();

	bool setData(int width, int height, int csp, int numOfPlane, int stride[], uint8_t *plane[], int64_t pts);

	int width() const { return m_width; }
	int height() const { return m_height; }
	int csp() const { return m_csp; }
	int64_t pts() const { return m_pts; }
	uint8_t *plane(int index) const {
		if(index < m_numOfPlane) {
			return m_plane[index]; 
		}
		return NULL;
	}

private:
	int m_width;
	int m_height;
	int m_csp;
	int m_numOfPlane;
	int m_stride[4];
	uint8_t *m_plane[4];
	int64_t m_pts;
};

class VideoEncodedSample
{
public:
	VideoEncodedSample();

	int size() const { return m_size; }
	uint8_t *data() const { return m_data; }

	void setData(const uint8_t *data, int size);

private:
	int m_size;
	uint8_t *m_data;
};

class VideoEncoder
{
protected:
	VideoEncoder();
	virtual ~VideoEncoder();

public:
	static VideoEncoder *create(const char *codecName);

public:
	virtual int encode(const VideoSample &sampleIn);
	virtual VideoEncodedSample *getOutput();
};