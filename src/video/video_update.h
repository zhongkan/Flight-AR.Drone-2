/***********************************************************************************
 * @file    video_update.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2013-03-29
 * @brief   Update the the video in main window 
 ***********************************************************************************/
#ifndef VIDEO_UPDATE_H
#define VIDEO_UPDATE_H

#include <QtCore/QThread>
#include <QtGui/QImage>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QObject>
#include <QtGui/QApplication>
#include <QtCore/QSemaphore>
#include <opencv2/opencv.hpp>
#include "..//video/video.h"
#include "..//video/video_frame.h"
#include "..//entry/mainWindow.h"

class videoUpdate : public QThread
{
	Q_OBJECT
public:
	videoUpdate(MainWindow * wd, Video* video, QObject* parent = 0);

	/* Convert the IplImage to QImage */
	QImage IplImageToQImage(const IplImage* iplImage);

	/* start / stop the thread */
	void go();
	void stop();


private:
	Video			*_video;
	MainWindow		*_wd;
	bool			_stop; 

	QTimer			*_videoTimer;

	IplImage		*_img;

	QSemaphore		*_sem;

	bool			_videoOk;

protected:
	void run();

signals:

	/* notify video that video_update thread had stoped */
	void videoUpdateStoped();

public slots:

	/* start / stop video update thread, receive signal from video */
	void startVideoUpdate();
	void stopVideoUpdate();
};

#endif // VIDEO_UPDATE_H