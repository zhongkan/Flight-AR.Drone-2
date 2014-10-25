#include "..//video/video.h"

Video::Video(ARDrone * ardrone, const QString ipAddr /* = "192.168.1.1" */, QObject *parent /* = 0 */)
	:QThread(parent)
{
	this->_ardrone		= ardrone;
	this->_ipAddrStr	= ipAddr;
	this->_stop			= true;
	this->_video_update_stop = true;;

	/* Video */
	_formatCtx			= NULL;
	_codecCtx			= NULL;
	_frame				= NULL;
	_frameBGR			= NULL;
	_bufferBRG			= NULL;
	_convertCtx			= NULL;	

	/* Semaphore */
	_sem = new QSemaphore(1);

}


/* Initialize the video stream, return A_OK if succeed, just for ardrone 2.0 */
RESULT Video::initVideoStream()
{
	/* Register all formats and codecs */
	av_register_all();
	avformat_network_init();
	av_log_set_level(AV_LOG_QUIET);

	/* Open the ip address and port */
	char filename[256];
	sprintf(filename, "tcp://%s:%d", _ipAddrStr.toStdString().c_str(), VIDEO_PORT);
	if (avformat_open_input(&_formatCtx, filename, NULL, NULL) < 0)
	{
		this->print("avformat_open_input() FAILED!");
		return A_FAILED;
	}

	/* Retrieve and dump stream information */
	avformat_find_stream_info(_formatCtx, NULL);
	av_dump_format(_formatCtx, 0, filename, 0);

	/* Find the decoder for the video stream */
	_codecCtx = _formatCtx->streams[0]->codec;
	AVCodec *pCodec = avcodec_find_decoder(_codecCtx->codec_id);
	if (pCodec == NULL)
	{
		this->print("avcodec_find_decoder() FAILED!");
		return A_FAILED;
	}

	/* Open codec */
	if (avcodec_open2(_codecCtx, pCodec, NULL) < 0)
	{
		this->print("avcodec_open2() FAILED!");
		return A_FAILED;
	}

	/* Allocate video frames and a buffer */
	_frame		= avcodec_alloc_frame();
	_frameBGR	= avcodec_alloc_frame();
	_bufferBRG	= (uint8_t*)av_mallocz(avpicture_get_size(PIX_FMT_BGR24, _codecCtx->width, _codecCtx->height) * sizeof(uint8_t));

	/* Assign appropriate parts of buffer to image planes in _frameBGR */
	avpicture_fill((AVPicture*)_frameBGR, _bufferBRG, PIX_FMT_BGR24, _codecCtx->width, _codecCtx->height);

	/* Convert it to BGR */
	_convertCtx = sws_getContext(_codecCtx->width, _codecCtx->height, _codecCtx->pix_fmt, _codecCtx->width, _codecCtx->height, PIX_FMT_BGR24, SWS_SPLINE, NULL, NULL, NULL);


	/* Allocate an IplImage */
	_img = cvCreateImage(cvSize(_codecCtx->width, _codecCtx->height), IPL_DEPTH_8U, 3);
	if (!_img)
	{
		this->print("cvCreateImage() FAILED!");
		return A_FAILED;
	}

	/* Clear the image */
	cvZero(_img);

	return A_OK;
}

/* Get ardrone's video stream */
RESULT Video::getVideo()
{
	AVPacket	packet;
	int			frameFinished = 0;

	/* Read all the frames */
	while (av_read_frame(_formatCtx, &packet) >= 0)
	{
		/* Decode  the frame */
		avcodec_decode_video2(_codecCtx, _frame, &frameFinished, &packet);

		/* Decode all the frames */
		if (frameFinished)
		{
			/* Convert to BGR */
			_sem->acquire();
			sws_scale(_convertCtx, (const uint8_t* const*)_frame->data, _frame->linesize, 0, _codecCtx->height, _frameBGR->data, _frameBGR->linesize);
			_sem->release();

			/* Free the packet and break immediately */
			av_free_packet(&packet);
			break;
		}

		/* Free the packet */
		av_free_packet(&packet);
	}

	return A_OK;
}

/* Get an image from the AR.Drone's camera */
IplImage* Video::getImage()
{
	/* No image */
	if (!_img) return NULL;

	/* Copy image to the IplImage */
	_sem->acquire();
	memcpy(_img->imageData, _frameBGR->data[0], _codecCtx->width * _codecCtx->height * sizeof(uint8_t) * 3);
	_sem->release();

	return _img;
}

/* start the thread */
void Video::go()
{
	this->_stop = false;
	this->start();
}

/* stop the thread */
void Video::stop()
{
	this->_stop = true;
}

void Video::finalizeVideo()
{
	if (_img)
	{
		cvReleaseImage(&_img);
		_img = NULL;
	}

	/* Deallocate the frame */
	if (_frame)
	{
		av_free(_frame);
		_frame = NULL;
	}

	/* Deallocate the frame */
	if (_frameBGR)
	{
		av_free(_frameBGR);
		_frameBGR = NULL;
	}

	/* Deallocate the buffer */
	if (_bufferBRG)
	{
		av_free(_bufferBRG);
		_bufferBRG = NULL;
	}

	/* Deallocate the convert context */
	if (_convertCtx)
	{
		sws_freeContext(_convertCtx);
		_convertCtx = NULL;
	}

	/* Deallocate the  codec */
	if (_codecCtx)
	{
		avcodec_close(_codecCtx);
		_codecCtx = NULL;
	}

	/* Deallocate the format context */
	if (_formatCtx)
	{
		avformat_close_input(&_formatCtx);
		_formatCtx = NULL;
	}
}

void Video::print(const string mesg)
{
	this->_ardrone->print(mesg);
}

void Video::print(const string mesg, const RESULT res)
{
	this->_ardrone->print(mesg, res);
}
void Video::print(const QString mesg)
{
	this->_ardrone->print(mesg);
}

void Video::print(const QString mesg, const RESULT res)
{
	this->_ardrone->print(mesg, res);
}

void Video::print(const char *mesg)
{
	this->_ardrone->print(mesg);
}

void Video::print(const char * mesg, const RESULT res)
{
	this->_ardrone->print(mesg, res);
}

void Video::videoStart()
{
	this->go();
}

void Video::videoStop()
{
	emit this->stopVideoUpdate();
	this->stop();
}

void Video::videoUpdateStoped()
{
	this->_video_update_stop = true;
}

void Video::run()
{
	RESULT res;

	/* Initialize the video stream */
	res = initVideoStream();
	if (SUCCEEDED(res))
	{
		print("Video initialize OK!");
		emit this->initVideoStreamRes(VIDEO_INIT_OK);
		emit this->startVideoUpdate();
		this->_video_update_stop = false;
	}
	else
	{
		print("Video initialize FAILED!");
		emit this->initVideoStreamRes(VIDEO_INIT_FAILED);
		return;
	}

	
	while(!_stop || !_video_update_stop)
	{
		/* Get the video stream */
		if (!getVideo()) break;
		msleep(1);
	}

	/* Finalize video */
	finalizeVideo();
}
