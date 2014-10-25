/***********************************************************************************************
 * @file    navData.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2012-03-29
 * @brief   This thread is used to receive video stream send by ardrone
 * Thanks to puku0, this part referenced puku0's project CV Drone(OpenCV + AR.Drone).
 * Anybody can redistribute or modify it under the LGPL, we just hope it's useful for
 * people to learn it, use it, it's total free and none-commercial
 *
 * The codes of decoding H.264 video is based on following sites.
 * - An ffmpeg and SDL Tutorial - Tutorial 01: Making Screen caps -
 *   http://dranger.com/ffmpeg/tutorial01.html
 * - AR.Drone Development - 2.1.2 AR.Drone 2.0 Video Decoding: FFMPEG + SDL2.0 -
 *   http://ardrone-ailab-u-tokyo.blogspot.jp/2012/07/212-ardrone-20-video-decording-ffmpeg.html
 ***********************************************************************************************/

#ifndef VIDEO_H
#define VIDEO_H
#include <stdio.h>
#include <stdlib.h>

// Qt
#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtCore/QtGlobal>
#include <QtCore/QSemaphore>

// OpenCV
#include <opencv2/opencv.hpp>

// FFmpeg
extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
}

#include "../common/ardrone.h"


class Video : public QThread
{
	Q_OBJECT
public:
    Video(ARDrone * ardrone, const QString ipAddr = "192.168.1.1", QObject *parent = 0);

	/* Initialize the ardrone video stream */
    RESULT initVideoStream();

	/* Get the ardrone video */
	RESULT getVideo();

	/* Get image from camera */
	IplImage* getImage();

	/* Finalize video */
	void finalizeVideo();

	/* start or stop the thread */
	void go();
	void stop();

	/* Print message to the main window */
	void print(const string mesg);
	void print(const string mesg, const RESULT res);

	void print(const QString mesg);
	void print(const QString mesg, const RESULT res);

	void print(const char * mesg);
	void print(const char * mesg, const RESULT res);

private:

	/* Current ardrone */
    ARDrone			*_ardrone;

	/* Ardrone ip address */
	QString			_ipAddrStr;
	
	/* Camera image */
	IplImage		*_img;

	/* Video */
	AVFormatContext	*_formatCtx;
	AVCodecContext	*_codecCtx;
	AVFrame			*_frame, *_frameBGR;
	quint8			*_bufferBRG;
	SwsContext		*_convertCtx;

	/* when set to true, the thread will be shutdown */
	bool			_stop;

	QSemaphore		*_sem;

	/* indicate the video_update thread stoped or not  */
	bool			_video_update_stop;

protected:
	void run();

signals:
	/* notify the main window that video initialize ok/failed */
	void initVideoStreamRes(int res);

	/* notify the video_update thread to start/stop */
	void startVideoUpdate();
	void stopVideoUpdate();


public slots:
	/* receive signal from thread ardrone, start/stop video thread */
	void videoStart();
	void videoStop();

	/* receive signal from video_update thread, indicate that video_update thread had stoped */
	void videoUpdateStoped();

};

#endif // VIDEO_H
