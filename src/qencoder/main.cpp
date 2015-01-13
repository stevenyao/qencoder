#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QTime>
#include "stdint.h"

extern "C"
{
	#include "x264.h"
}

#include "VideoEncoder.h"

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	VideoEncoder *e = VideoEncoder::create("x264");

	QFile f("../../test/lol_720p_548fr.yuv");
	f.open(QIODevice::ReadOnly);

	QFile out("../../test/lol.264");
	out.open(QIODevice::ReadWrite | QIODevice::Truncate);

	QTime beginTime = QTime::currentTime();

	qint64 frameCount = f.size() / 1280 / 720 / 3 * 2;

	uint8_t *plane[4];
	plane[0] = new uint8_t[1280 * 720];
	plane[1] = new uint8_t[1280 * 720 / 4];
	plane[2] = new uint8_t[1280 * 720 / 4];
	plane[3] = 0;

	int stride[4];
	stride[0] = 1280;
	stride[1] = 1280 / 2;
	stride[2] = 1280 / 2;
	stride[3] = 0;
	int pts = 1;
	for(int i = 0; i < frameCount; i++) {
		QByteArray data = f.read(720 * 1280 * 3 / 2);

		memcpy(plane[0], data.data(), 720 * 1280);
		memcpy(plane[1], data.data() + 720 * 1280, 720 * 1280 / 4);
		memcpy(plane[2], data.data() + 720 * 1280 * 5 / 4, 720 * 1280 / 4);

		VideoSample sampleIn;
		sampleIn.setData(1280, 720, X264_CSP_I420, 3, stride, plane, pts++);

		if(e->encode(sampleIn)) {
			VideoEncodedSample *sampleOut = e->getOutput();
			if(sampleOut) {
				out.write((const char*)sampleOut->data(), sampleOut->size());
				qDebug() << i << " sample size = " << sampleOut->size() << " bytes";
			}
		}
	}

	QTime endTime = QTime::currentTime();
	qDebug() << "encoding fps = " << frameCount * 1000 / beginTime.msecsTo(endTime);

	out.close();
	f.close();
}
