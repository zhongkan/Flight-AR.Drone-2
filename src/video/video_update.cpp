#include "video_update.h"

videoUpdate::videoUpdate(MainWindow *wd, Video* video, QObject* parent /* = 0 */)
	:QThread(parent)
{
	this->_wd = wd;
	this->_video = video;
	this->_stop	 = false;
	this->_videoOk = false;
	this->_sem = new QSemaphore(1);
}

QImage videoUpdate::IplImageToQImage(const IplImage* iplImage)
{   
	QImage qimage((uchar*)iplImage->imageData, iplImage->width, iplImage->height, iplImage->widthStep, QImage::Format_RGB888);
	return qimage.rgbSwapped();
}

void videoUpdate::go()
{
	this->_stop = false;
	this->start();
}

void videoUpdate::stop()
{
	this->_stop = true;
}

void videoUpdate::startVideoUpdate()
{
	this->go();
}

void videoUpdate::stopVideoUpdate()
{
	this->stop();
}

void videoUpdate::run()
{
	int width = 0;
	int height = 0;

	while(!_stop)
	{
		/* wait for 30 ms */
		QTime t;
		t.start();
		while(t.elapsed() < 30)
			QCoreApplication::processEvents();

		/* get the width and height of the video area */
		width = _wd->_video_frame->getWidth();
		height = _wd->_video_frame->getHeight();
		
		/* get a video frame */
		 _img = _video->getImage();

		 /* convert the image */
		 QImage img = IplImageToQImage(_img);

		 /* scale the image */
		 //QImage qimg = img.scaled(width * 4, height * 4).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		 /* show the video frame */
		 this->_wd->_video_frame->updateImage(img);

		 /* show the image */
		 //cvShowImage("camera", _img);
		 //cvWaitKey(1);
	}

	emit videoUpdateStoped();

} 