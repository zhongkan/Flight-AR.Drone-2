#include "..//video/video_frame.h"

videoFrame::videoFrame(int width /* = 720 */, int height /* = 360 */, QWidget *parent /* = 0 */)
	:QWidget(parent)
{
	this->_width = width;
	this->_height = height;
	this->_sem = new QSemaphore(1);

	/* Initialize the frame */
	this->setBackgroundRole(QPalette::Dark);
	this->setMinimumSize(width, height);
	this->setAutoFillBackground(true);
	QObject::connect(this, SIGNAL(draw()), this, SLOT(repaint()));
}

videoFrame::~videoFrame()
{
	QObject::disconnect(this, SIGNAL(draw()), this, SLOT(repaint()));
}

int videoFrame::getWidth()
{
	return this->_width;
}

int videoFrame::getHeight()
{
	return this->_height;
}


void videoFrame::updateImage(QImage img)
{
	_sem->acquire();
	_img = img;
	_sem->release();
	emit this->draw();
}

void videoFrame::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	_sem->acquire();
	painter.drawImage(0, 0, _img);
	_sem->release();
}

void videoFrame::snapShot()
{
	QDateTime time = QDateTime::currentDateTime();
	QString str = time.toString("yyyy-MM-dd-hh-mm-ss");
	
	QFile file(str + ".png");

	if(!file.open(QIODevice::ReadWrite))
	{
		return;
	}
	
	QByteArray ba;  
	QBuffer buffer(&ba);  
	buffer.open(QIODevice::WriteOnly); 
	_sem->acquire();
	_img.save(str + ".png", "PNG");
	_sem->release();
	file.write(ba);
}